    var all = function(list, element, before)
    {
        var out = [];
        var found = false;
        for (var i = 0 ; i < list.length ; i++)
        {
            found = found || (list[i] == element);
            if (before && !found) out.push(list[i]);
            if (!before && found && (list[i] != element)) out.push(list[i]);
        }
        return out;
    }

    var hide = function(element)
    {
        $('#' + element).css('display', 'none');
    }

    var show = function(element)
    {
        $('#' + element).css('display', 'block');
    }

    var all_before = function(list, element)
    {
        return all(list, element, true);
    }

    var all_after = function(list, element)
    {
        return all(list, element, false);
    }


