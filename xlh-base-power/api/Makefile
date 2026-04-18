HOST=192.168.1.151
USER=xlh
PASSWORD=xlh
SERVICE=xlh_api.service
SERVICE_SM=xlh_rgb_shared_memory.service

build:
	sudo rm -rf .venv && sudo rm -rf uv.lock && uv sync --upgrade
	sudo systemctl restart ${SERVICE}
	sudo systemctl restart ${SERVICE_SM}

update:
	uv lock --upgrade && uv sync
	sudo systemctl restart ${SERVICE}
	sudo systemctl restart ${SERVICE_SM}

status:
	sudo systemctl status ${SERVICE}

start:
	sudo systemctl start ${SERVICE}

stop:
	sudo systemctl stop ${SERVICE}

restart:
	sudo systemctl restart ${SERVICE}

enable:
	sudo systemctl enable ${SERVICE}

disable:
	sudo systemctl disable ${SERVICE}

status_sm:
	sudo systemctl status ${SERVICE_SM}

start_sm:
	sudo systemctl start ${SERVICE_SM}

stop_sm:
	sudo systemctl stop ${SERVICE_SM}

restart_sm:
	sudo systemctl restart ${SERVICE_SM}

enable_sm:
	sudo systemctl enable ${SERVICE_SM}

disable_sm:
	sudo systemctl disable ${SERVICE_SM}

reload:
	sudo systemctl daemon-reload

log:
	journalctl -xeu ${SERVICE} -f

log_sm:
	journalctl -xeu ${SERVICE_SM} -f

log_all:
	journalctl -f

ssh_remove:
	ssh-keygen -f "/home/phe/.ssh/known_hosts" -R "${HOST}"

ssh_dev: ssh_upload
	 sshpass -p "${PASSWORD}" ssh "${USER}@${HOST}" "$(MAKE) -C ~/xemax/api restart"
	 sshpass -p "${PASSWORD}" ssh "${USER}@${HOST}" "$(MAKE) -C ~/xemax/api restart_sm"

ssh_upload:
	sshpass -p "${PASSWORD}" rsync -av --delete -e "ssh -o StrictHostKeyChecking=no" \
		--exclude .venv \
		--exclude .claude \
		--exclude .idea \
		--exclude .python-version \
		--exclude uv.lock \
		./ ${USER}@${HOST}:xemax/api