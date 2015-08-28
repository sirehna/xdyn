
function get_plotter()
{
    var latest_t = 0;
    totalPoints = 300;
    var full_plot_data = {};
    var number_of_different_colors = 20;

    function idx2color(i)
    {
        return jQuery.Color({ hue: (i*200/number_of_different_colors), saturation: 0.95, lightness: 0.35, alpha: 1 }).toHexString();
    }

    function create_dataset(yaml,variable,col)
    {
        var hex_color = idx2color(col);
        return {label: variable, data: [[t, yaml[variable]]], color: hex_color};
    }

    function append(yaml)
    {
        var t = yaml['t'];
        var col = 0;
        if (t < latest_t)
        {
            latest_t = 0;
            full_plot_data = {};
        }
        $.each(yaml, function(key, val)
                     {
                        if (key != 't')
                        {
                            if (full_plot_data[key])
                            {
                                full_plot_data[key]["data"].push([t,val]);
                            }
                            else
                            {
                                full_plot_data[key] = create_dataset(yaml,key,col);
                                col++;
                            }
                        }
                     });
        latest_t = t;
        return full_plot_data;
    }


    var plot = $.plot($("#graph"), full_plot_data);

    function data_was_received_from_websocket_at_this_instant(t)
    {
        return (typeof t != 'undefined');
    }

    var plot_selector_container = $("#plot_selector");
    function insert_checkboxes()
    {
        var i = 0;
        $.each(full_plot_data, function(key, val) {
        plot_selector_container.append("<div style='font-color: black; background-color: " + idx2color(i) + ";'>" +
                                "<input type='checkbox' name='" + key + "' checked='checked' id='id" + key + "' style=></input>" +
                                "<label for='id" + key + "'>" + val.label + "</label></div>");
        i++;
        });
        plot_selector_container.find("input").click(plot_selected_curves);
    }

    function plot_selected_curves()
    {
        plot_options = {
                            legend: {show: true, container: '#legend_container'},
                            series: {shadowSize: 0}
                       };

        var selected_data_to_plot = [];

        plot_selector_container.find("input:checked").each(function ()
                                                    {
                                                        var key = $(this).attr("name");
                                                        if (key && full_plot_data[key]) selected_data_to_plot.push(full_plot_data[key]);
                                                    });

        if (selected_data_to_plot.length > 0)
        {
            $.plot($("#graph"), selected_data_to_plot, plot_options);
        }
    }


   
    var t = NaN;
    var f = function plot_yaml(yaml_data, ship_name, variable_to_plot)
    {
        var first_run = isNaN(t);
        t = yaml_data['t'];
        y = yaml_data[variable_to_plot + '(' + ship_name + ')']
        if (data_was_received_from_websocket_at_this_instant(t))
        {
                full_plot_data = append(yaml_data)
                if (first_run) insert_checkboxes();
                plot_selected_curves();
        }
        if (yaml_data.hasOwnProperty('waves'))
        {
            waves = yaml_data['waves'];
            console.log(base91Float32.decode(waves['z']));
        }
    }
    return f;
}
