
var next_color = function()
{
    color_map = ['#ff0000', '#00ff00', '#0000ff'];
    idx = 0;
    return function()
    {
        col = color_map[idx];
        if (idx == 2)
        {
            idx = 0;
        }
        else
        {
            idx = idx+1;
        }
        return col;    
    }
}()



var plot = function(map,name)
{
    lineX.append(map['t'], map['x(' + name + ')']);
}

