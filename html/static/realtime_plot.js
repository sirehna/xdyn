
function get_plotter()
{
    totalPoints = 300;
    data = [];
    function append(t,x)
    {
        if (data.length == totalPoints)
        {
            data = data.slice(1);
        }
        data.push([t,x]);
        return data;
    }

    var plot = $.plot($("#graph"), [ data ]);
    latest_t = 0;
    var f = function plot_yaml(yaml_data, ship_name, variable_to_plot)
    {
        t = yaml_data['t'];
        y = yaml_data[variable_to_plot + '(' + ship_name + ')']
        if (typeof t != 'undefined')
        {
                if (t<latest_t)
                {
                    data = [];
                }

                console.log(variable_to_plot + "(" + ship_name + ") = " + y);
                plot.setData([append(t,y)]);
                plot.setupGrid();
                plot.draw();
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
