# -*- coding: utf-8 -*-
from report import GraphFactory
import os
import functools


def prepare_plot_data(data, name="", **kwargs):
    """
    :param data: the pandas dataframe
    :param name: the name of the plot, this information will be used later in the graph legend
    :param kwargs: Depending on arguments provided, the representation changes. If x and y are provided, it's cartesian.
    :return: Returns a "plot". A plot is a dictionnary with two fields:
             - data_plot : contains data to display (instance of DataPlot class)
             - plot_params : knows how data has to be displayed (instance of PlotParams class)

    Example :
    To define a plot with a cartesian representation, call in jinja script :

    prepare_plot_data(dataframe, name='name for legend', x = 'a', y = 'b')
    (If there is at least two columns a and b in a dataframe)
    """
    plot = {"data_plot": GraphFactory.DataPlot(data, **kwargs)}
    args_plot_params = {}
    if 'x' in kwargs and 'y' in kwargs:
        args_plot_params.update({"names": ["x", "y"]})
        args_plot_params.update({"grepr": "Cart"})
    if name:
        args_plot_params.update({"name": name})
    plot.update({"plot_params": GraphFactory.PlotParams(**args_plot_params)})
    return plot


def plot_bar(x, y, name=""):
    data = {"x": x, "y": y}
    plot = {"data_plot": GraphFactory.DataPlot(data, x="x", y="y")}
    args_plot_params = {"names": ["bins", "values"], "grepr": "Bar", "name": name}
    plot.update({"plot_params": GraphFactory.PlotParams(**args_plot_params)})
    return plot


def orig(plot):
    new_plot = plot
    new_plot["data_plot"].data_source = plot["data_plot"].data_source.copy()
    if "x" in new_plot["plot_params"].names and "y" in new_plot["plot_params"].names:
        key = new_plot["data_plot"].values["x"]
        x0 = new_plot["data_plot"].data_source[key].iloc[0]
        new_plot["data_plot"].data_source[key] -= x0
    else:
        raise NotImplementedError

    return new_plot


def cartesian_graph(plots, **kwargs):
    graph = GraphFactory.Graph(grepr="Cart", **kwargs)
    for plot in plots:
        graph.add_plot(data_plot=plot["data_plot"], plot_params=plot["plot_params"])
    return graph


def create_layout(size=(1, 1), working_directory=os.getcwd(), img_format='.svg', **kwargs):
    glayout = GraphFactory.GridLayout(size=size, **kwargs)
    image = GraphFactory.Image(glayout)
    path_image = image.create_image(working_directory, "pict_{}".format(glayout.id), img_format)
    relative_path = os.path.split(path_image)[1]
    return "![{0}]({1}){{#fig:id}}".format(kwargs.get('title', ''), relative_path)


def api(params):
    wd = params["working_directory"]
    img_format = params["image_format"]
    return {"prepare_plot_data": prepare_plot_data,
            "cartesian_graph": cartesian_graph,
            "layout": functools.partial(create_layout, working_directory=wd, img_format=img_format),
            "orig": orig,
            "prepare_plot_bar": plot_bar}
