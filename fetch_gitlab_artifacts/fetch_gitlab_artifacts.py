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

    # Gitlab API handler
    gl = gitlab.Gitlab('https://gitlab.sirehna.com', private_token, ssl_verify=False)
    print('Gitlab API V{0}'.format(gl.api_version))
    project = gl.projects.get(project_id)
    pipelines = project.pipelines.list(all=True)
    if commit:
        pipelines = [p for p in pipelines if p.attributes['sha'] == commit]
        if len(pipelines) != 1:
            logging.error("No pipeline found")
            raise Exception
    else:
        pipelines = [p for p in pipelines if p.attributes['ref'] == 'master']
    pipeline = pipelines[0]
    if pipeline.attributes['status'] != 'success':
        logging.error("Unsucessful build")
        raise Exception
    jobs = pipeline.jobs.list(all=True)
    job_names = [job.attributes['name'] for job in jobs]
    search_for_job = 'build:' + build_type
    jobs = [job for job in jobs if job.attributes['name'] == search_for_job]
    if len(jobs) != 1:
        logging.error("Job {0} was not found ".format(search_for_job))
        raise Exception
    job = jobs[0]
    job_for_artifacts = project.jobs.get(job.get_id(), lazy=True)
    # Download latest artifact
    download_artifact(job_for_artifacts)


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
        )

