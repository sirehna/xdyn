$(function() {
    name = "Anthineas";
    var_to_plot = "z";
    plotter = get_plotter();

    var gui_elements = ["stlfilechooser", "solver", "outputs", "run_button"];

    $("#yaml_upload").change(function() {
        if(this.files.length)
        {
            var reader = new FileReader();

            reader.onload = function(e)
            {
                var parsed_message = jsyaml.load(e.target.result);
                console.log(parsed_message);
                if (parsed_message['bodies'])
                {
                    var mesh = parsed_message['bodies'][0]['mesh'];
                    if (mesh)
                    {
                        show("stlfilechooser");
                    }
                    else
                    {
                        hide("stlfilechooser");
                        $.map(all_after(gui_elements, "stlfilechooser"), show);
                    }
                }
            };

            reader.readAsBinaryString(this.files[0]);
        }
        else
        {
            hide("stlfilechooser");
            $.map(all_after(gui_elements, "stlfilechooser"), show);
        }
        show("yaml_file_name");
    });
    
    $("#stl_upload").change(function() {
        $.map(all_after(gui_elements, "stlfilechooser"), show);
    });

    
    $('select').material_select();
    

    function set_plug_state(svg, state)
    {
        // Change the color
        svg.select("#bottom-pin").attr("class", state);
        svg.select("#left-chord").attr("class", state);
        svg.select("#top-pin").attr("class", state);
        svg.select("#left-connector").attr("class", state);
        svg.select("#right-connector").attr("class", state);
        svg.select("#right-chord").attr("class", state);

        // Put the two connectors together
        var offset = 0;
        if (state == "connected")
        {
            offset = 10;
        }
        svg.select("#right-connector").transform("t-"+offset+", 0");
        svg.select("#left-connector").transform("t"+offset+", 0");
        svg.select("#bottom-pin").transform("t"+offset+", 0");
        svg.select("#top-pin").transform("t"+offset+", 0");
        svg.select("#left-chord").transform("t"+offset+", 0");
        svg.select("#right-chord").transform("t-"+offset+", 0");
    }

    function register_connection_state(state)
    {
        var s = Snap('#plug');
        $('[id="connection_status"]').html(state);
        set_plug_state(s, state);
    }

    $.map(gui_elements, hide);
    hide("graph");
    window.WebSocket = window.WebSocket || window.MozWebSocket;
    var address = $('#websocket_address').html();
    var websocket = new WebSocket(address);
    websocket.onopen = function () {
        $('h1').css('color', '#1e88e5');
        register_connection_state("connected");
        show("filechooser");
    };

    websocket.onerror = function () {
        $('h1').css('color', 'red');
        register_connection_state("disconnected");
    };
    websocket.onmessage = function (message) {
        show("graph");
        try
        {
            var parsed_message = jsyaml.load(message.data);
            plotter(parsed_message, name, var_to_plot);
        }
        catch(err)
        {
            console.log(err.message)
            console.log('Cannot parse ' + message.data)
        }
    };
});
