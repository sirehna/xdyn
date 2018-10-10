import re


def equation_substitution(text):
    eq_inline = re.compile('\$`(.*?)`\$')
    eq_multiline = re.compile('^```math(.*?)^```', re.MULTILINE | re.DOTALL)
    new_text = re.sub(eq_inline, r'$\1$', text)
    new_text = re.sub(eq_multiline, r'$$\1$$  {#eq:}', new_text)
    return new_text


def equation_substitution2(text):
    eq_multiline = re.compile('\n\$\$(.*?)\$\$', re.MULTILINE | re.DOTALL)
    new_text = re.sub(eq_multiline, r'$$\1$$ {#eq:}\n', text)
    return new_text


def generate_metadata(**kwargs):
    from textwrap import dedent
    metadata = r"""
    ---
    title: Simulateur X-Dyn
    author: Sirehna
    numbersections: true
    eqnos-cleveref: true
    eqnos-plus-name: Eq.
    xnos-number-sections: true
    link-citations: true
    ---
    """
    return dedent(metadata)


def convert_file_for_pandoc(filename,
    filename_output):
    if filename_output is None or len(filename_output) == 0:
        filename_output = filename + '.pandoc'
    lines = open(filename, 'r').readlines()
    content = '\n'.join(lines[2:])
    f = open(filename_output, 'w')
    content = equation_substitution2(content)
    content = generate_metadata() + content
    f.write(content)
    f.close()


if __name__=='__main__':
    import argparse
    parser = argparse.ArgumentParser()
    pa = parser.add_argument
    pa('input_filename', type=str, help='')
    pa('-o', '--output', type=str,
       help='Name of the output result file',
       default='')
    args = parser.parse_args()
    convert_file_for_pandoc(args.input_filename, args.output)

