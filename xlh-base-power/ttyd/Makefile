HOST=192.168.1.31
USER=xlh
PASSWORD=xlh
SERVICE=xlh_tty_shell.service

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
	 sshpass -p "${PASSWORD}" ssh "${USER}@${HOST}" "$(MAKE) -C ~/xemax/ttyd restart"

ssh_upload:
	sshpass -p "${PASSWORD}" rsync -av -e "ssh -o StrictHostKeyChecking=no" \
		./ ${USER}@${HOST}:xemax/ttyd