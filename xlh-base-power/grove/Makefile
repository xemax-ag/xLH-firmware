HOST=192.168.1.169
USER=xlh
PASSWORD=xlh
SERVICE=xlh_grove.service

build:
	sudo rm -rf .venv && sudo rm -rf uv.lock && uv sync

update:
	uv lock --upgrade && uv sync

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

reload:
	sudo systemctl daemon-reload

log:
	journalctl -xeu ${SERVICE} -f

log_all:
	journalctl -f

ssh_upload:
	sshpass -p "${PASSWORD}" rsync -av --delete -e "ssh -o StrictHostKeyChecking=no" \
		--exclude .venv \
		--exclude .idea \
		--exclude codesys \
		--exclude misc \
		--exclude uv.lock \
		./ ${USER}@${HOST}:xemax/grove