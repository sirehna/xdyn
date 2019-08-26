.PHONY: all

all:
	@CURRENT_UID=$(shell id -u):$(shell id -g) \
	docker-compose \
		up \
		-t 0 \
        --exit-code-from test \
        --abort-on-container-exit \
        --build

