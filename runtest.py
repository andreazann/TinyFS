from os.path import basename
from subprocess import PIPE, run, Popen, CalledProcessError

import click


tests_folder = 'tests'
bin_folder = 'bin'


@click.group()
@click.option('--v', '--verbose', is_flag=True, help='Prints additional output.')
@click.pass_context
def cli(ctx, verbose):
    ctx.obj['verbose'] = verbose


@cli.command()
@click.argument('src', nargs=-1)
@click.argument('dst')
@click.pass_context
def aggregate(ctx, src, dst):
    """
    Creates a single file from [{src}]
    :param src: The files to aggregate
    :param dst: The destination file
    """
    verbose = ctx.obj['verbose']
    click.echo(src)

    execute(['frama-c', '-quiet'] + list(src) + ['-print', '>', dst + '.raw'])
    execute(['awk', '\'/\\/\\* Generated by Frama-C \\*\\//{i++}i\'', dst + '.raw', '>', dst])
    execute(['awk', '\'/int printf_va_[0-9]+\\(char const \\*format.*\\);/{i++}i\'', dst, '>', dst + '.raw'])
    execute(['awk', '\'{gsub(\"printf_va_[0-9]+\", \"printf\")}1\'', dst + '.raw', '>', dst])
    execute(['awk', '\'{gsub(\"__fc_stdin\", \"stdin\")}1\'', dst, '>', dst + '.raw'])
    execute(['awk', '\'/extern int \( \/\* missing proto \*\/ getline\)\(char \*\*x_0, size_t \*x_1, FILE \*x_2\);/{'
                    'i++}i\'', dst + '.raw', '>', dst])
    execute(['rm', dst + '.raw'])


@cli.command()
@click.argument('src')
@click.argument('dst', default=None)
@click.argument('cmd', default=None, required=False)
def build(src, dst, cmd=None):
    """
    Builds the file passed as argument.\n
    :param src: The file to build.\n
    :param dst: The path for the generated executable. Default: ' + bin_folder + '/{src}'\n
    :param cmd: A custom build command. Default: 'gcc -o {dst} {src}'
    """
    dst = dst if dst is not None else bin_folder + '/' + basename(src)
    cmd = cmd if cmd is not None else 'gcc -o ' + dst + ' ' + src

    for text in execute(cmd.split(' ')):
        print(text, end="")


@cli.command()
@click.argument('test_name')
def test(test_name):
    """
    Runs the requested test.
    :param test_name: The name of the test to run.
    """
    click.echo('Testing %s' % test_name)


def execute(cmd):
    """
    Executes command
    :param cmd: A list containing [command name, arg0, arg1, arg2...]
    :return:
    """
    pipe = Popen(cmd, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    for stdout_line in iter(pipe.stdout.readline, ""):
        yield stdout_line
    pipe.stdout.close()
    return_code = pipe.wait()
    if return_code:
        raise CalledProcessError(return_code, cmd)


if __name__ == '__main__':
    cli(obj={})
