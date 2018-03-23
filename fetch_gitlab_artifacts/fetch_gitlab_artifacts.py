import gitlab
from dateutil.parser import parse
import logging
import urllib3

# Shut the f*** up
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

service_name = "fetch-gitlab-artifacts"

logging.basicConfig(format='%(asctime)s,%(msecs)d [' + service_name + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S',
    level=logging.INFO)

def get_latest_successful_build(builds):
    """
    this function returns the latest successful Gitlab build on the target branch
    """
    latest = None
    for build in builds:
        if latest is None:
            latest = build
        else:
            latest_build_date = parse(latest.finished_at)
            current_build_date = parse(build.finished_at)
            if current_build_date > latest_build_date:
                latest = build
    return latest

def download_artifact(latest):
    # Handler for downloads
    class Download(object):
        def __init__(self):
            self._fd = open('artifacts.zip', 'wb')

        def __call__(self, chunk):
            self._fd.write(chunk)

    if latest is not None:
        # Download artifact as a stream
        target = Download()
        if hasattr(latest, 'artifacts'):
            latest.artifacts(streamed=True, action=target)
            del(target)  # flushes data on disk
            logging.info("Downloaded artifacts.zip")
        else:
            logging.error("Build was found, but it does not contain any artifacts")

    else:
        logging.error("No build found so no artifact downloaded")

def fetch_artifact(**kwargs):
    """
    this function fetchs artifact from a given project with optionnally the id
    of the git commit
    """
    project_id = kwargs.get('project_id', 0)
    private_token = kwargs.get('private_token', 'xxxxxxxxxxxxxxxxxxxx')
    commit = kwargs.get('commit', None)
    build_type = kwargs.get('build_type', '')
    target_branch = kwargs.get('target_branch', 'master')
    stage = kwargs.get('stage', 'build')


    # Gitlab API handler
    gl = gitlab.Gitlab('https://gitlab.sirehna.com', private_token, ssl_verify=False)

    # Get all builds: you can get the ID from https://gitlab.sirehna.com/root/scientific_computing/edit
    builds = gl.project_builds.list(project_id=project_id, all=True)

    # Select only successful builds from the target stage, from the target branch
    if commit:
        if build_type:
            builds = [b for b in builds if b.status == "success" and b.stage == stage and b.commit is not None and b.commit.id == commit and b.name.endswith(build_type)]
        else:
            builds = [b for b in builds if b.status == "success" and b.stage == stage and b.commit is not None and b.commit.id == commit]
    else:
        if build_type:
            builds = [b for b in builds if b.status == "success" and b.stage == stage and b.ref == target_branch]
        else:
            builds = [b for b in builds if b.status == "success" and b.stage == stage and b.ref == target_branch and b.name.endswith(build_type)]

    # Get latest stable build on target branch
    latest = get_latest_successful_build(builds)

    # Download latest artifact
    download_artifact(latest)


def command_line_arguments_parser():
    import argparse
    default_project = 42
    default_commit = None
    default_private_token = 'BvC3JDEocCPxVZN4Wuxd'
    default_build_type = ''
    default_branch = 'master'
    default_stage = 'build'
    parser = argparse.ArgumentParser(description='fetch artifact from a Gitlab builds')
    pa = parser.add_argument
    pa('-p', '--project_id', default=default_project, help="Gitlab's project ID. You can find this on the project's settings page (eg. https://gitlab.sirehna.com/ThirdParty/ipopt/edit). Default is {0} (SSC).".format(default_project))
    pa('-c', '--commit', default=default_commit, help='Sha to be used. Default is {0}'.format(default_commit))
    pa('-t', '--private_token', default=default_private_token, help='Private token to be used to connect to gitlab. Default is {0}'.format(default_private_token))
    pa('-b', '--build_type', default=default_build_type, help='Build type. Default is "{0}"'.format(default_build_type))
    pa('-a', '--target_branch', default=default_branch, help='Branch of which we want the artifacts. Default is "{0}"'.format(default_branch))
    pa('-s', '--build_stage', default=default_stage, help='Gitlab build stage (defined in .gitlab-ci.yml). Default is "{0}"'.format(default_stage))
    return parser


def getParserArgs():
    parser = command_line_arguments_parser()
    return parser.parse_args()


if __name__ == "__main__":
    args = getParserArgs()
    fetch_artifact(
        project_id = args.project_id,
        private_token = args.private_token,
        commit = args.commit,
        build_type = args.build_type,
        target_branch = args.target_branch,
        stage = args.build_stage
        )


