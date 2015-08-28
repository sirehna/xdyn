
function get_plotter()
{
    totalPoints = 300;
    var full_plot_data = {};

    function create_dataset(yaml,variable)
    {
        return {label: variable, data: [[t, yaml[variable]]]};
    }

    function append(yaml)
    {
        var t = yaml['t'];
        $.each(yaml, function(key, val)
                     {
                     try{
                        if (key != 't')
                        {
                            if (full_plot_data[key])
                            {
                                full_plot_data[key]["data"].push([t,val]);
                            }
                            else
                            {
                                full_plot_data[key] = create_dataset(yaml,key);
                            }
                        }
                        }
                        catch(err)
                        {
                            console.log(err);
                        }
                     });
        return full_plot_data;
    }


    var plot = $.plot($("#graph"), full_plot_data);

    function data_was_received_from_websocket_at_this_instant(t)
    {
        return (typeof t != 'undefined');
    }

    var f = function plot_yaml(yaml_data, ship_name, variable_to_plot)
    {
        t = yaml_data['t'];
        y = yaml_data[variable_to_plot + '(' + ship_name + ')']
        if (data_was_received_from_websocket_at_this_instant(t))
        {
                full_plot_data = append(yaml_data)
                var selected_data_to_plot = full_plot_data['z(Anthineas)'];
                selected_data_to_plot.color = 1;
                $.plot($("#graph"), [selected_data_to_plot]);
        }
        if (yaml_data.hasOwnProperty('waves'))
        {
            waves = yaml_data['waves'];
            console.log(base91Float32.decode(waves['z']));
        }
    }
    return f;
}
