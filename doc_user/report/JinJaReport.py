# -*- coding: utf-8 -*-
from jinja2 import FileSystemLoader, Environment
import tempfile
import shutil
import os
import subprocess
from report import ApiGraphJinja
from report import ApiDataJinja
from report import ApiDataFormatting
from report import ApiJinjaCommands


class ReportGenerator:

    def __init__(self, searchpath_templates):
        self._loader = FileSystemLoader(searchpath=searchpath_templates)
        self._env = Environment(loader=self._loader)

    def add_functions(self, functions):
        self._env.globals.update(**functions)

    def generate(self, template_name, optional_variables={}):
        template = self._env.get_template(template_name)
        return template.render(**optional_variables)

    def write_report(self, file_path, template_name, optional_variables={}):
        with open(file_path, "w", encoding='utf-8') as f:
            f.write(self.generate(template_name, optional_variables))


def define_parser():
    import argparse
    parser = argparse.ArgumentParser()
    pa = parser.add_argument
    pa("-p", "--path-template",
       dest="path_template",
       type=str,
       help="absolute template path",
       required=True)
    pa("-r", "--report-name",
       dest="report_name",
       type=str,
       help="name of the report",
       required=True)
    pa("-k", "--keep-md",
       dest="keep_md",
       action="store_true",
       help="Option to keep the generated markdown report",
       default=False)
    pa("-v", "--verbose",
       action='store_true',
       help="Display the instructions")
    pa("-i", "--intermediate_files",
       dest="ifiles",
       action="store_true",
       help="Option to keep intermediate files",
       default=False)
    pa("-f", "--required_files",
       dest="required_files",
       nargs="+",
       type=str,
       default=[])
    return parser


def get_args(cli=None):
    parser = define_parser()
    return parser.parse_args(cli)


def create_environment_for_report(original_path, files_required):
    """
    :param original_path: template absolute path (ex: /home/foo/template.md)
    :param files_required: List of files needed, specified by user. These files are copied in the temporary directory.
    :return:
    """
    rep = tempfile.TemporaryDirectory()
    path_dir_temp = rep.name
    original_path = os.path.normpath(original_path)
    copy_path = original_path
    report_extensions = [".jin", ".md", ".rep"]
    files_to_copy = files_required + list(filter(lambda x: os.path.splitext(x)[1] in report_extensions, os.listdir(original_path)))
    return rep, original_path, copy_path


def compile_markdown_with_pandoc(input_filename, output_filename, working_directory):
    """
    :param input_filename: Name of the markdown file to compile
    :param output_filename: Name of the document to create (it's important to specify the extension !)
    :param working_directory: Temporary directory absolute path. This directory must contain the markdown file and the compiled document will be created here.
    :return:
    """
    custom_params = {".html": ["--self-contained"]}
    output_filename_extension = os.path.splitext(output_filename)[1]
    custom_args = " ".join(custom_params[".html"]) if output_filename_extension in custom_params else ""
    cmd_line = "pandoc -s {} -o {} {}".format(input_filename, output_filename, custom_args).strip()
    p = subprocess.Popen(cmd_line, cwd=working_directory, shell=True)
    p.wait()


def get_generated_image_extension_from_requested_report(report_name):
    """
    :param report_name: Name of the report with extension (ex : report.docx, report.html, ...)
    :return:
    The format of pictures created in the report depend on the file extension. For example, with .docx it's not possible to have .svg files.
    """
    imageExtension = {'.pdf': '.svg', '.docx': '.png', ".html": ".svg"}
    ext = os.path.splitext(report_name)[1].lower()
    imageExtension = imageExtension.get(ext, '.svg')
    return imageExtension


def append_api(gen, params_api):
    """
    :param gen: Instance of class ReportGenerator
    :param params_api: Dictionnary with fields : verbose, image_format, working_directory, original_path
    :return:
    """
    functions_ApiGraphJinja = ApiGraphJinja.api(params_api)
    functions_ApiDataJinja = ApiDataJinja.api(params_api)
    functions_ApiDataFormatting = ApiDataFormatting.api(params_api)
    functions_ApiJinjaCommands = ApiJinjaCommands.api(params_api)
    gen.add_functions(functions_ApiGraphJinja)
    gen.add_functions(functions_ApiDataJinja)
    gen.add_functions(functions_ApiDataFormatting)
    gen.add_functions(functions_ApiJinjaCommands)


def create_report(path_template, report_name, keep_md=False, optional_variables={},
                  verbose=False, keep_intermediate_files=False, files_required=[]):
    infos = {}
    report_filename = "report.md"
    params_api = {"verbose": verbose}
    path_rep_templates, name_template = os.path.split(path_template)
    params_api["image_format"] = get_generated_image_extension_from_requested_report(report_name)
    rep, original_path, copy_path = create_environment_for_report(path_rep_templates, files_required)
    params_api["working_directory"] = copy_path
    params_api["original_path"] = original_path
    infos.update(original_path=original_path, copy_path=copy_path)
    gen = ReportGenerator(path_rep_templates)
    append_api(gen, params_api)
    report_path = os.path.join(copy_path, report_filename)
    gen.write_report(file_path=report_path,
                     template_name=name_template,
                     optional_variables=optional_variables)
    compile_markdown_with_pandoc(report_filename, report_name, copy_path)

    if keep_md:
        path_temp_report = os.path.join(copy_path, report_filename)
        path_dest_temp_report = os.path.join(path_rep_templates, report_filename)
        infos.update(path_temp_report=path_temp_report, path_dest_temp_report=path_dest_temp_report)

    files_to_retrieve = []
    if keep_intermediate_files:
        files_to_retrieve.append(".h5")

    if files_to_retrieve:
        retrieve_work(copy_path, path_rep_templates, files_to_retrieve)
    rep.cleanup()
    return infos


def retrieve_work(path_temp_report, path_dest_report, extensions):
    """
    :param path_temp_report:
    :param path_dest_report:
    :param extensions:
    :return:
    """
    content = os.listdir(path_temp_report)
    files = filter(lambda x: os.path.splitext(x)[1] in extensions, content)


def main(cli=None):
    arguments = get_args(cli=cli)
    infos = create_report(arguments.path_template, arguments.report_name, arguments.keep_md, verbose=arguments.verbose,
                          keep_intermediate_files=arguments.ifiles, files_required=arguments.required_files)
    if arguments.verbose:
        print("Copying tree '" + infos["original_path"] + "' in : '" + infos["copy_path"] + "'")
        if arguments.keep_md:
            print('Copy report_filename : ' + infos["path_temp_report"] + ' -> ' + infos["path_dest_temp_report"])


if __name__ == "__main__":
    main()
