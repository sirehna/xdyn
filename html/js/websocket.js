
var dataLength = 500; // number of dataPoints visible at any point
var dps = []; // dataPoints
var get_new_canvas = function()
{
    dp_x = []; // dataPoints
    dp_y = []; // dataPoints
    dp_z = []; // dataPoints
    chart = new CanvasJS.Chart("chartContainer",{
            title :{
                text: "Simulated data"
            },          
            data: [{
                type: "spline",
                showInLegend: true, 
                name: "x",
                legendText: "x",
                dataPoints: dp_x
            },
            {
                type: "spline",
                showInLegend: true, 
                name: "y",
                legendText: "y",
                dataPoints: dp_y 
            },
            {
                type: "spline",
                showInLegend: true, 
                name: "z",
                legendText: "z",
                dataPoints: dp_z 
            }
]
        });
}

        var updateChart = function (t, map, name) {
                dp_x.push({
                    x: t,
                    y: map['x('+name+')']
                });
                dp_y.push({
                    x: t,
                    y: map['y('+name+')']
                });
                dp_z.push({
                    x: t,
                    y: map['z('+name+')']
                });
/*            if (dp_x.length > dataLength)
            {
                dps.shift();                
            }*/
            chart.render();     
            };


$(function() {

    var name = 'cube';

    window.WebSocket = window.WebSocket || window.MozWebSocket;
    var websocket = new WebSocket('ws://127.0.0.1:9002');
    websocket.onopen = function () {
        $('h1').css('color', 'green');
        get_new_canvas();
    };
    websocket.onerror = function () {
        $('h1').css('color', 'red');
    };
    websocket.onmessage = function (message) {
           var parsed_message = jsyaml.load(message.data);
            t = parsed_message['t'];
            x = parsed_message['x(' + name + ')']
            y = parsed_message['y(' + name + ')']
            z = parsed_message['z(' + name + ')']
           if (typeof t != 'undefined')
           {
                updateChart(t,parsed_message,name);
           }
    };
            });

