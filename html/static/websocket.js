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

    var plot = $.plot($("#placeholder"), [ data ], { yaxis: { max: 1 }});
    latest_t = 0;

    function change_state(svg, state)
    {
       svg.select("#bottom-pin").attr("class", state);
       svg.select("#left-chord").attr("class", state);
       svg.select("#top-pin").attr("class", state);
       svg.select("#left-connector").attr("class", state);
       svg.select("#right-connector").attr("class", state);
       svg.select("#right-chord").attr("class", state);

       // Translate
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

    function set_plug_state(state)
    {
        var s = Snap('#plug');
        change_state(s, state);
    }

    window.WebSocket = window.WebSocket || window.MozWebSocket;
    //var websocket = new WebSocket('ws://130.66.124.225:9003');
    var websocket = new WebSocket('ws://localhost:9002');
    websocket.onopen = function () {
        $('h1').css('color', 'green');
        set_plug_state("connected");
    };
    websocket.onerror = function () {
        $('h1').css('color', 'red');
        set_plug_state("disconnected");
    };
    websocket.onmessage = function (message) {
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
    };
});
