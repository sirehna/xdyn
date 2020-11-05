/*
 * RadiationDampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include "RadiationDampingForceModel.hpp"

#include "Body.hpp"
#include "HDBParser.hpp"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "RadiationDampingBuilder.hpp"
#include "external_data_structures_parsers.hpp"

#include <ssc/macros.hpp>
#include <ssc/text_file_reader.hpp>

#include <ssc/yaml_parser.hpp>

#include "yaml.h"

#include <cassert>
#include <array>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

std::string RadiationDampingForceModel::model_name() {return "radiation damping";}

class CSVWriter
{
    public:
        CSVWriter(std::ostream& os_, const std::string& x_name_, const std::vector<double>& x) : os(os_), x_name(x_name_), xs(x), functions(), function_names()
        {
        }
        CSVWriter& operator=(const CSVWriter& rhs);
        ~CSVWriter(){};
        CSVWriter(const CSVWriter&);

        void add(const std::string& name, const std::function<double(const double)>& f, const size_t i, const size_t j)
        {
            std::stringstream ss;
            ss << name << '_' << i << j;
            function_names.push_back(ss.str());
            functions.push_back(f);
        }

        void print()
        {
            print_title();
            print_values();

        }

    private:
        CSVWriter();

        void print_title()
        {
            os << x_name;
            for (auto f:function_names)
            {
                os << ',' << f;
            }
            os << std::endl;
        }

        void print_values()
        {
            for (auto x:xs)
            {
                os << x;
                for (auto f:functions)
                {
                    os << ',' << f(x);
                }
                os << std::endl;
            }
        }
        std::ostream& os;
        std::string x_name;
        std::vector<double> xs;
        std::vector<std::function<double(const double)> > functions;
        std::vector<std::string> function_names;

};

class RadiationDampingForceModel::Impl
{
    public:
        Impl(const TR1(shared_ptr)<HDBParser>& parser, const YamlRadiationDamping& yaml) : hdb{parser}, builder(RadiationDampingBuilder(yaml.type_of_quadrature_for_convolution, yaml.type_of_quadrature_for_cos_transform)), K(),
        omega(parser->get_radiation_damping_angular_frequencies()), taus(), n(yaml.nb_of_points_for_retardation_function_discretization), Tmin(yaml.tau_min), Tmax(yaml.tau_max),
        H0(yaml.calculation_point_in_body_frame.x,yaml.calculation_point_in_body_frame.y,yaml.calculation_point_in_body_frame.y)
        {
            CSVWriter omega_writer(std::cerr, "omega", omega);
            taus = builder.build_regular_intervals(Tmin,Tmax,n);
            CSVWriter tau_writer(std::cerr, "tau", taus);

            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    const auto Br = get_Br(i,j);
                    K[i][j] = get_K(Br);
                    if (yaml.output_Br_and_K)
                    {
                        omega_writer.add("Br",Br,i+1,j+1);
                        tau_writer.add("K",K[i][j],i+1,j+1);
                    }
                }
            }
            if (yaml.output_Br_and_K)
            {
                std::cerr << "Debugging information for damping functions Br:" << std::endl;
                omega_writer.print();
                std::cerr << std::endl << "Debugging information for retardation functions K:" << std::endl;
                tau_writer.print();
            }
        }

        std::function<double(double)> get_Br(const size_t i, const size_t j) const
        {
            return builder.build_interpolator(omega,hdb->get_radiation_damping_coeff(i,j));
        }

        std::function<double(double)> get_K(const std::function<double(double)>& Br) const
        {
            return builder.build_retardation_function(Br,taus,1E-3,omega.front(),omega.back());
        }

        double get_convolution_for_axis(const size_t i, const BodyStates& states)
        {
            double K_X_dot = 0;
            for (size_t k = 0 ; k < 6 ; ++k)
            {
            	const History his = get_velocity_history_from_index(k, states);
                if (his.get_duration() >= Tmin)
                {
                    // Integrate up to Tmax if possible, but never exceed the history length
                    const double co = builder.convolution(his, K[i][k], Tmin, std::min(Tmax, his.get_duration()));
                    K_X_dot += co;
                }
            }
            return K_X_dot;
        }

        History get_velocity_history_from_index(const size_t i, const BodyStates& states) const
        {
        	switch(i)
        	{
        		case 0: return states.u;
        		case 1: return states.v;
        		case 2: return states.w;
        		case 3: return states.p;
        		case 4: return states.q;
        		case 5: return states.r;
        		default: return History();
        	}
        }

        ssc::kinematics::Wrench get_wrench(const BodyStates& states)
        {
            ssc::kinematics::Vector6d W;
            const ssc::kinematics::Point H(states.name,H0);

            W(0) = -get_convolution_for_axis(0, states);
            W(1) = -get_convolution_for_axis(1, states);
            W(2) = -get_convolution_for_axis(2, states);
            W(3) = -get_convolution_for_axis(3, states);
            W(4) = -get_convolution_for_axis(4, states);
            W(5) = -get_convolution_for_axis(5, states);
            return ssc::kinematics::Wrench(states.name,W);
        }

        double get_Tmax() const
        {
            return Tmax;
        }

    private:
        Impl();
        TR1(shared_ptr)<HDBParser> hdb;
        RadiationDampingBuilder builder;
        std::array<std::array<std::function<double(double)>,6>, 6> K;
        std::vector<double> omega;
        std::vector<double> taus;
        size_t n;
        double Tmin;
        double Tmax;
        Eigen::Vector3d H0;
};


RadiationDampingForceModel::RadiationDampingForceModel(const RadiationDampingForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& ) : ForceModel("radiation damping", body_name_),
pimpl(new Impl(input.hdb, input.yaml))
{
}

double RadiationDampingForceModel::get_Tmax() const
{
    return pimpl->get_Tmax();
}

ssc::kinematics::Wrench RadiationDampingForceModel::operator()(const BodyStates& states, const double ) const
{
    return pimpl->get_wrench(states);
}

TypeOfQuadrature parse_type_of_quadrature_(const std::string& s);
TypeOfQuadrature parse_type_of_quadrature_(const std::string& s)
{
    if      (s == "gauss-kronrod")   return TypeOfQuadrature::GAUSS_KRONROD;
    else if (s == "rectangle")       return TypeOfQuadrature::RECTANGLE;
    else if (s == "simpson")         return TypeOfQuadrature::SIMPSON;
    else if (s == "trapezoidal")     return TypeOfQuadrature::TRAPEZOIDAL;
    else if (s == "burcher")         return TypeOfQuadrature::BURCHER;
    else if (s == "clenshaw-curtis") return TypeOfQuadrature::CLENSHAW_CURTIS;
    else if (s == "filon")           return TypeOfQuadrature::FILON;
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unkown quadrature type: " << s << ". Should be one of 'gauss-kronrod', 'rectangle', ' simpson', 'trapezoidal', 'burcher', 'clenshaw-curtis' or 'filon'.";);
    }
    return TypeOfQuadrature::FILON;
}

RadiationDampingForceModel::Input RadiationDampingForceModel::parse(const std::string& yaml, const bool parse_hdb)
{
    RadiationDampingForceModel::Input ret;
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlRadiationDamping input;
    node["hdb"] >> input.hdb_filename;
    std::string s;
    node["type of quadrature for cos transform"] >> s;
    input.type_of_quadrature_for_cos_transform = parse_type_of_quadrature_(s);
    node["type of quadrature for convolution"] >> s;
    input.type_of_quadrature_for_convolution = parse_type_of_quadrature_(s);
    node["nb of points for retardation function discretization"] >> input.nb_of_points_for_retardation_function_discretization;
    ssc::yaml_parser::parse_uv(node["omega min"], input.omega_min);
    ssc::yaml_parser::parse_uv(node["omega max"], input.omega_max);
    ssc::yaml_parser::parse_uv(node["tau min"], input.tau_min);
    ssc::yaml_parser::parse_uv(node["tau max"], input.tau_max);
    node["output Br and K"] >> input.output_Br_and_K;
    node["calculation point in body frame"] >> input.calculation_point_in_body_frame;
    if (parse_hdb)
    {
        const TR1(shared_ptr)<HDBParser> hdb(new HDBParser(ssc::text_file_reader::TextFileReader(std::vector<std::string>(1,input.hdb_filename)).get_contents()));
        ret.hdb = hdb;
    }
    ret.yaml = input;
    return ret;
}
