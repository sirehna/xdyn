
function get_plotter()
{
    totalPoints = 300;
    var dd = {};

    function create_dataset(yaml,variable)
    {
        return {label: variable, data: [[t, yaml[variable]]]};
    }

    function apnd(yaml)
    {
        var t = yaml['t'];
        $.each(yaml, function(key, val)
                     {
                     try{
                        if (key != 't')
                        {
                            if (dd[key])
                            {
                                dd[key]["data"].push([t,val]);
                            }
                            else
                            {
                                dd[key] = create_dataset(yaml,key);
                            }
                        }
                        }
                        catch(err)
                        {
                            console.log(err);
                        }
                     });
        return dd;
    }


    var plot = $.plot($("#graph"), dd);
    latest_t = 0;
    var f = function plot_yaml(yaml_data, ship_name, variable_to_plot)
    {
        t = yaml_data['t'];
        y = yaml_data[variable_to_plot + '(' + ship_name + ')']
        if (typeof t != 'undefined')
        {
                if (t<latest_t)
                {
                    dd = {};
                }
                dd = apnd(yaml_data)
                var D = dd['z(Anthineas)'];
                D.color = 1;
                 $.plot($("#graph"), [D]);
                latest_t = t;
        }
        if (yaml_data.hasOwnProperty('waves'))
        {
            waves = yaml_data['waves'];
            console.log(base91Float32.decode(waves['z']));
        }
    }
    return f;
}
