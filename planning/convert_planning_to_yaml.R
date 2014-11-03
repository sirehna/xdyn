library(yaml)
L = yaml.load_file('kanban.yml')

get_story = function(stories, index)
{
    return(stories$stories[index][[1]])
}

get_task = function(story, index)
{
    return(story$tasks[[index]])
}

get_nb_of_stories = function(stories)
{
    return(length(L$stories))
}

get_nb_of_tasks = function(story)
{
    return(length(story$tasks))
}

get_total_nb_of_tasks = function(stories)
{
    N = get_nb_of_stories(stories)
    nb_of_stories = 0
    for (i in 1:N)
    {
        nb_of_stories = nb_of_stories + get_nb_of_tasks(get_story(stories, i))
    }
    return(nb_of_stories)
}

convert_to_date = function(d)
{
    return(as.Date(d, format="%d/%m/%Y"))
}

is_empty <- function(x)
{
    return(length(x)==0)
}

robustify_for_string = function(x)
{
    if (is_empty(x))
    {
        return("")
    }
    return(x)
}

robustify_for_dates = function(x)
{
    if (is_empty(x))
    {
        return(NA)
    }
    return(x)
}

robustify_for_numeric = function(x)
{
    if (is_empty(x))
    {
        return(NA)
    }
    return(x)
}

get_data_frame = function(stories)
{
    N = get_total_nb_of_tasks(stories)
    story = character(N)
    due_by = as.Date(numeric(N), origin="1900-01-01")
    task = character(N)
    size = numeric(N)
    type = character(N)
    backlog = as.Date(numeric(N), origin="1900-01-01")
    started = as.Date(numeric(N), origin="1900-01-01")
    done = as.Date(numeric(N), origin="1900-01-01")
    assigned_to = character(N)
    k = 1
    nb_of_stories = get_nb_of_stories(stories)
    for (i in 1:nb_of_stories)
    {
        s = get_story(stories, i)
        nb_of_tasks = get_nb_of_tasks(s)
        for (j in 1:nb_of_tasks)
        {
            t = get_task(s, j)
            story[k] = s$story
            due_by[k] = convert_to_date(robustify_for_dates(t$started))
            task[k] = robustify_for_string(t$task)
            size[k] = robustify_for_numeric(t$size)
            type[k] = robustify_for_string(t$type)
            backlog[k] = convert_to_date(robustify_for_dates(t$backlog))
            started[k] = convert_to_date(robustify_for_dates(t$started))
            done[k] = convert_to_date(robustify_for_dates(t$done))
            print(t$task)
            assigned_to[k] = robustify_for_string(t$"assigned to")
            k = k+1
        }
    }
    return(data.frame(story, due_by, task, size, type, backlog, started, done, assigned_to, stringsAsFactors=FALSE))
}

