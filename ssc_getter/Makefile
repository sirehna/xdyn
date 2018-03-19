all:
	docker build --build-arg UID=$(shell id -u) --build-arg GID=$(shell id -g) --build-arg USER=$(shell whoami) -t fetch_gitlab_artifacts .
	docker run -t --rm -v $(shell pwd):/out --net=host -e USER=$(shell whoami) -e UID=$(shell id -u) -e GID=$(shell id -g) --net=host fetch_gitlab_artifacts --project_id 42 --target_branch master --build_stage=build
	unzip artifacts.zip
	rm artifacts.zip
