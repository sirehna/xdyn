
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

    var legend_container = $("#legend");
    function insert_checkboxes()
    {
        $.each(full_plot_data, function(key, val) {
        legend_container.append("<br/>" +
                                "<input type='checkbox' name='" + key + "' checked='checked' id='id" + key + "'></input>" +
                                "<label for='id" + key + "'>" + val.label + "</label>");
        });
        legend_container.find("input").click(plot_selected_curves);
    }


    function plot_selected_curves()
    {

        var selected_data_to_plot = [];

        legend_container.find("input:checked").each(function ()
                                                    {
                                                        var key = $(this).attr("name");
                                                        if (key && full_plot_data[key]) selected_data_to_plot.push(full_plot_data[key]);
                                                    });

        if (selected_data_to_plot.length > 0)
        {
            $.plot($("#graph"), selected_data_to_plot);
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
