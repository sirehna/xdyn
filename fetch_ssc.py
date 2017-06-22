import gitlab
import dateutil.parser


# Gitlab API handler
gl = gitlab.Gitlab('https://gitlab.sirehna.com', 'BvC3JDEocCPxVZN4Wuxd', ssl_verify=False)

# Get all SSC builds (project 42): you can get project ID from https://gitlab.sirehna.com/root/scientific_computing/edit
builds = gl.project_builds.list(project_id=42)

# Get latest stable build on master
latest = None
target_branch = 'dev/ci_docker'
for build in builds:
    if latest is None:
        if build.ref == target_branch and build.status == "success":
            latest = build
    else:
        latest_build_date = dateutil.parser.parse(latest.created_at)
        current_build_date = dateutil.parser.parse(build.created_at)
        if build.ref == target_branch and build.status == "success" and current_build_date > latest_build_date:
            latest = build

# Handler for downloads
class Download(object):
    def __init__(self):
        self._fd = open('artifacts.zip', 'wb')

    def __call__(self, chunk):
        self._fd.write(chunk)

print(latest)
if latest is not None:
    # Download artifact as a stream
    target = Download()
    latest.artifacts(streamed=True, action=target)
    del(target)  # flushes data on disk
    print("Downloaded artifacts.zip")
    import zipfile
    zip_ref = zipfile.ZipFile('artifacts.zip', 'r')
    zip_ref.extractall('.')
    zip_ref.close()
    print("Extracted artifacts.zip")
else:
    print("No build found so no artefact downloaded")

