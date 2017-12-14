import gitlab
from dateutil.parser import parse
import logging

service_name = "ssc-getter"

def get_build_from_commit(builds, commit, **kwargs):
    """
    given a full commit SHA, this function returns the lastest successful
    gitlab build
    """
    build_type = kwargs.get('build_type', '')
    verbose = kwargs.get('verbose', False)
    if build_type:
        build_type = 'build:' + build_type
    else:
        build_type = 'build'
    list_build_with_correct_commit = []
    for build in builds:
        if build.commit is not None and build.commit.id == commit and build.name.startswith(build_type):
            list_build_with_correct_commit.append(build)
    logging.debug(len(list_build_with_correct_commit))
    logging.debug(list_build_with_correct_commit)
    if list_build_with_correct_commit:
        latest = list_build_with_correct_commit[0]
        for b in list_build_with_correct_commit:
            if parse(b.finished_at) < parse(latest.finished_at):
                latest = b
    else:
        latest = None
    if verbose:
        print(latest)
    return latest


def get_latest_master_build(builds, **kwargs):
    """
    this function returns the lastest successful gitlab build on master branch
    """
    build_type = kwargs.get('build_type', '')
    target_branch = kwargs.get('target_branch', 'master')
    verbose = kwargs.get('verbose', False)
    if build_type:
        build_type = 'build:' + build_type
    else:
        build_type = 'build'
    builds = [build for build in builds if build.name.startswith(build_type)]
    latest = None
    for build in builds:
        if latest is None:
            if build.ref == target_branch:
                latest = build
        else:
            latest_build_date = parse(latest.finished_at)
            current_build_date = parse(build.finished_at)
            if build.ref == target_branch and current_build_date > latest_build_date:
                latest = build
    return latest

def fetch_artifact(**kwargs):
    """
    this function fetchs artifact from a given project with optionnally the id
    of the git commit
    """
    project_id = kwargs.get('project_id', 0)
    private_token = kwargs.get('private_token', 'xxxxxxxxxxxxxxxxxxxx')
    commit = kwargs.get('commit', None)
    build_type = kwargs.get('build_type', '')

    # Gitlab API handler
    gl = gitlab.Gitlab('https://gitlab.sirehna.com', private_token, ssl_verify=False)

    # Get all SSC builds (project 42): you can get project ID from https://gitlab.sirehna.com/root/scientific_computing/edit
    builds = gl.project_builds.list(project_id=project_id, all=True)

    # Seletect only successful builds
    builds = [b for b in builds if b.status == "success"]


    if commit:
        # Get latest stable build from commit sha
        latest = get_build_from_commit(builds, commit, build_type=build_type)
    else:
        # Get latest stable build on master
        latest = get_latest_master_build(builds, build_type=build_type)

    # Handler for downloads
    class Download(object):
        def __init__(self):
            self._fd = open('artifacts.zip', 'wb')

        def __call__(self, chunk):
            self._fd.write(chunk)

    logging.debug(latest)
    if latest is not None:
        # Download artifact as a stream
        target = Download()
        latest.artifacts(streamed=True, action=target)
        del(target)  # flushes data on disk
        logging.info("Downloaded artifacts.zip")
    else:
        logging.error("No build found so no artifact downloaded")


def getParser():
    import argparse
    default_project = 42
    default_commit = None
    default_private_token = 'BvC3JDEocCPxVZN4Wuxd'
    default_build_type = ''
    parser = argparse.ArgumentParser(description='fetch artifact from a Gitlab builds')
    pa = parser.add_argument
    pa('-p', '--project_id', default=default_project, help='Sha . Default is {0}'.format(default_project))
    pa('-c', '--commit', default=default_commit, help='Sha to be used. Default is {0}'.format(default_commit))
    pa('-t', '--private_token', default=default_private_token, help='Private token to be used to connect to gitlab. Default is {0}'.format(default_private_token))
    pa('-b', '--build_type', default=default_build_type, help='Build type. Default is "{0}"'.format(default_build_type))
    return parser


def getParserArgs(cli=None):
    parser = getParser()
    return parser.parse_args(cli)


def main(cli=None):
    args = getParserArgs(cli)
    fetch_artifact(
        project_id = args.project_id,
        private_token = args.private_token,
        commit = args.commit,
        build_type = args.build_type)


if __name__ == "__main__":
    main()
