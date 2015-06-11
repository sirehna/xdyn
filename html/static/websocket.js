$(function() {
    data = [];
    name = "cube";
    totalPoints = 300;
    function append(t,x)
    {
        if (data.length == totalPoints)
        {
            data = data.slice(1);
        }
        data.push([t,x]);
        return data;
    }

    var plot = $.plot($("#graph"), [ data ], { yaxis: { max: 1 }});
    latest_t = 0;
    
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
                        $('#stlfilechooser').css('visibility', 'visible');
                        //$('#stl_button').html('<span>Browse for ' + mesh.substr(0,2) + '</span>')
                    }
                    else
                    {
                        $('#solver_form').css('visibility', 'visible');
                    }
                }
            };

            reader.readAsBinaryString(this.files[0]);
        }
        else
        {
            $('#stlfilechooser').css('visibility', 'hidden');
            $('#solver_form').css('visibility', 'visible');
        }
        $('#yaml_file_name').css('visibility', 'visible');
    });
    
    $("#stl_upload").change(function() {
        $('#solver_form').css('visibility', 'visible');
    });
    
    $("#solver_chooser").change(function() {
        $('#run_button').css('visibility', 'visible');
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

    window.WebSocket = window.WebSocket || window.MozWebSocket;
    var websocket = new WebSocket('ws://130.66.124.225:9003');
    //var websocket = new WebSocket('ws://localhost:9002');
    $('#graph').css('visibility', 'hidden');
    $('#filechooser').css('visibility', 'hidden');
    $('#stlfilechooser').css('visibility', 'hidden');
    $('#yaml_file_name').css('visibility', 'hidden');
    $('#run_button').css('visibility', 'hidden');
    $('#solver_form').css('visibility', 'hidden');
    websocket.onopen = function () {
        $('h1').css('color', '#1e88e5');
        register_connection_state("connected");
        $('#filechooser').css('visibility', 'visible');
        //$('#stlfilechooser').css('visibility', 'hidden');
    };
    websocket.onerror = function () {
        $('h1').css('color', 'red');
        register_connection_state("disconnected");
    };
    websocket.onmessage = function (message) {
        $('#graph').css('visibility', 'visible');
        $('#filechooser#').css('visibility', 'hidden');
        try
        {
            var parsed_message = jsyaml.load(message.data);
            t = parsed_message['t'];
            x = parsed_message['x(' + name + ')']
            y = parsed_message['y(' + name + ')']
            z = parsed_message['z(' + name + ')']
            if (typeof t != 'undefined')
            {
                    if (t<latest_t)
                    {
                        data = [];
                    }
                    console.log("z = " + z);
                    plot.setData([append(t,z)]);
                    plot.setupGrid();
                    plot.draw();
                    latest_t = t;
            }
            if (parsed_message.hasOwnProperty('waves'))
            {
                waves = parsed_message['waves'];
                console.log(base91Float32.decode(waves['z']));
            }
        }
        catch(err)
        {
            console.log(err.message)
            console.log('Can not parse ' + message.data)
        }
    };
});
