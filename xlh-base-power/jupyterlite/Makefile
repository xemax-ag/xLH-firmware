HOST=192.168.1.31
USER=xlh
PASSWORD=xlh
SERVICE=xlh_jupyterlite.service

build:
	sudo rm -rf .venv && sudo rm -rf uv.lock && sudo rm -rf dist && uv sync --upgrade && \
	.venv/bin/jupyter lite build --output-dir dist

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

ssh_dev: ssh_upload
	 @echo "..."

ssh_upload:
	sshpass -p "${PASSWORD}" rsync -av -e "ssh -o StrictHostKeyChecking=no" \
		./ ${USER}@${HOST}:xemax/jupyterlite
