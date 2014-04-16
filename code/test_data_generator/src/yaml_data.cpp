/*
 * yaml_data.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "yaml_data.hpp"
#include <sstream>

std::string test_data::rotation()
{
    std::stringstream ss;
    ss << "rotations:" << std::endl
       << "    order by: angle" << std::endl
       << "    convention: [x,y',z'']";
    return ss.str();
}

std::string test_data::model1()
{
    std::stringstream ss;
    ss << "model: no waves" << std::endl
       << "frame: NED" << std::endl
       << "z: {value: 0, unit: m}" << std::endl;
    return ss.str();
}

std::string test_data::model2()
{
    std::stringstream ss;
    ss << "model: gravity" << std::endl
       << "g: {value: 9.81, unit: m/s^2}" << std::endl;
    return ss.str();
}

std::string test_data::hydrostatic_test()
{
    std::stringstream ss;
    ss << "rotations:\n"
       << "    order by: angle\n"
       << "    convention: [x,y',z'']\n"
       << "\n"
       << "environment:\n"
       << "  - model: no waves\n"
       << "    frame: NED\n"
       << "    z: {value: 0, unit: m}\n"
       << "  - model: no wind\n"
       << "    \n"
       << "# Fixed frame: NED\n"
       << "bodies: # All bodies have NED as parent frame\n"
       << "  - name: body 1\n"
       << "    mesh: anthineas.stl\n"
       << "    position of body frame relative to mesh:\n"
       << "        x: {value: 10, unit: m}\n"
       << "        y: {value: 0, unit: m}\n"
       << "        z: {value: 0, unit: m}\n"
       << "        phi: {value: 0, unit: rad}\n"
       << "        theta: {value: 0, unit: rad}\n"
       << "        psi: {value: 0, unit: rad}\n"
       << "    initial position of body frame relative to NED projected in NED:\n"
       << "        x: {value: 0, unit: m}\n"
       << "        y: {value: 0, unit: m}\n"
       << "        z: {value: 0, unit: m}\n"
       << "        phi: {value: 0, unit: rad}\n"
       << "        theta: {value: 0, unit: rad}\n"
       << "        psi: {value: 0, unit: rad}\n"
       << "    initial velocity of body frame relative to NED projected in body:\n"
       << "        u: {value: 0, unit: m/s}\n"
       << "        v: {value: 0, unit: m/s}\n"
       << "        w: {value: 0, unit: m/s}\n"
       << "        p: {value: 0, unit: rad/s}\n"
       << "        q: {value: 0, unit: rad/s}\n"
       << "        r: {value: 0, unit: rad/s}\n"
       << "    dynamics:\n"
       << "        position of centre of inertia:\n"
       << "            frame: body 1\n"
       << "            x: {value: 0, unit: m}\n"
       << "            y: {value: 0, unit: m}\n"
       << "            z: {value: 0, unit: m}\n"
       << "        mass: {value: 1000, unit: t}\n"
       << "        inertia matrix divided by mass:\n"
       << "            frame: body 1\n"
       << "            row 1: [1,0,0,0,0,0]\n"
       << "            row 2: [0,1,0,0,0,0]\n"
       << "            row 3: [0,0,1,0,0,0]\n"
       << "            row 4: [0,0,0,1,0,0]\n"
       << "            row 5: [0,0,0,0,1,0]\n"
       << "            row 6: [0,0,0,0,0,1]\n"
       << "    external forces:\n"
       << "      - model: gravity\n"
       << "        g: {value: 9.81, unit: m/s^2}\n"
       << "      - model: non-linear hydrostatic\n"
       << "    \n"
       << "\n"
       << "points:\n"
       << "  - name: P\n"
       << "    frame: body 1\n"
       << "    x: {value: 10, unit: m}\n"
       << "    y: {value: 10, unit: m}\n"
       << "    z: {value: 20, unit: m}\n"
       << "  - name: O\n"
       << "    frame: NED\n"
       << "    x: {value: 0, unit: m}\n"
       << "    y: {value: 0, unit: m}\n"
       << "    z: {value: 0, unit: m}\n"
       << "\n"
       << "blocked degrees of freedom body/NED->body:\n"
       << "   - body: body 1\n"
       << "     blocked: [x, theta]\n"
       << "    \n"
       << "outputs:\n"
       << "    positions:\n"
       << "      - point: O\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        axes: [x,z]\n"
       << "    angles:\n"
       << "      - frame: body 1\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        axes: [x,y,z]\n"
       << "    linear velocities:\n"
       << "      - point: P\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        axes: [x,y]\n"
       << "      - point: P\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: NED\n"
       << "        axes: [z]\n"
       << "    angular velocities:\n"
       << "      - frame: body 1\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        axes: [x,y]\n"
       << "    linear accelerations:\n"
       << "      - point: P\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        axes: [x,y]\n"
       << "      - point: P\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: NED\n"
       << "        axes: [z]\n"
       << "    angular accelerations:\n"
       << "      - frame: body 1\n"
       << "        relative to frame: NED\n"
       << "        projected in frame: body 1\n"
       << "        #  = p,q\n"
       << "        axes: [x,y]\n"
       << "    forces and torques:\n"
       << "      - type: non-linear hydrostatic\n"
       << "        point name: centre of inertia\n"
       << "        point frame: body 1\n"
       << "        projected in frame: NED\n"
       << "        axes: [X, Z, K, N]\n";
    return ss.str();
}
