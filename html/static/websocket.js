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
                        $('#stlfilechooser').css('display', 'block');
                    }
                    else
                    {
                        $('#stlfilechooser').css('display', 'none');
                        $('#solver').css('display', 'block');
                        $('#outputs').css('display', 'block');
                        $('#run_button').css('display', 'block');
                    }
                }
            };

            reader.readAsBinaryString(this.files[0]);
        }
        else
        {
            $('#stlfilechooser').css('display', 'none');
            $('#solver').css('display', 'block');
            $('#outputs').css('display', 'block');
            $('#run_button').css('display', 'block');
        }
        $('#yaml_file_name').css('display', 'block');
        //$('#stepsize').css('display', 'block');
    });
    
    $("#stl_upload").change(function() {
        $('#solver').css('display', 'block');
        $('#outputs').css('display', 'block');
        $('#run_button').css('display', 'block');
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
    $('#graph').css('display', 'none');
    $('#filechooser').css('display', 'none');
    $('#stlfilechooser').css('display', 'none');
    $('#yaml_file_name').css('display', 'none');
    $('#run_button').css('display', 'none');
    $('#solver').css('display', 'none');
    $('#outputs').css('display', 'none');
    websocket.onopen = function () {
        $('h1').css('color', '#1e88e5');
        register_connection_state("connected");
        $('#filechooser').css('display', 'block');
        //$('#stlfilechooser').css('display', 'none');
    };
    websocket.onerror = function () {
        $('h1').css('color', 'red');
        register_connection_state("disconnected");
    };
    websocket.onmessage = function (message) {
        $('#graph').css('display', 'block');
        $('#filechooser#').css('display', 'none');
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
