#!/bin/sh

### BEGIN INIT INFO
# Provides:	        MosqAlarm	
# Required-Start:	$remote_fs $syslog
# Required-Stop:	$remote_fs $syslog
# Default-Start:	2 3 4 5
# Default-Stop:		
# Short-Description:MosqAlarm
### END INIT INFO

EXEC=/usr/sbin/MosqAlarm
RUN_PID=/var/run/MosqAlarm.pid

APP_DESC="MosqAlarm"
APP_NAME=MosqAlarm

set -e

# /etc/init.d/mosquitto: start and stop the mosquitto MQTT message broker

test -x $EXEC || exit 0
#type $EXEC 2>/dev/null >/dev/null || exit 1

umask 022

. /lib/lsb/init-functions

# Are we running from init?
run_by_init() {
    ([ "$previous" ] && [ "$runlevel" ]) || [ "$runlevel" = S ]
}

export PATH="${PATH:+$PATH:}/usr/local/bin:/sbin"

case "$1" in
  start)
	log_daemon_msg "Starting $APP_DESC" "$APP_NAME"
	if start-stop-daemon --start --quiet --oknodo --make-pidfile --pidfile $RUN_PID --background --exec $EXEC -- ; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;
  stop)
	log_daemon_msg "Stopping $APP_DESC" "$APP_NAME"
	if start-stop-daemon --stop --quiet --oknodo --pidfile $RUN_PID; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;

  restart)
	log_daemon_msg "Restarting $APP_DESC" "$APP_NAME"
	start-stop-daemon --stop --quiet --oknodo --retry 30 --pidfile $RUN_PID
	#check_for_no_start log_end_msg
	#check_dev_null log_end_msg
	if start-stop-daemon --start --quiet --oknodo --pidfile $RUN_PID --background --exec $EXEC -- ; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;

  try-restart)
	log_daemon_msg "Restarting $APP_DESC" "$APP_NAME"
	set +e
	start-stop-daemon --stop --quiet --retry 30 --pidfile $RUN_PID
	RET="$?"
	set -e
	case $RET in
	    0)
		# old daemon stopped
		check_for_no_start log_end_msg
		check_dev_null log_end_msg
		if start-stop-daemon --start --quiet --oknodo --pidfile $RUN_PID --exec $EXEC -- ; then
		    log_end_msg 0
		else
		    log_end_msg 1
		fi
		;;
	    1)
		# daemon not running
		log_progress_msg "(not running)"
		log_end_msg 0
		;;
	    *)
		# failed to stop
		log_progress_msg "(failed to stop)"
		log_end_msg 1
		;;
	esac
	;;

  status)
	status_of_proc -p $RUN_PID $EXEC $APP_NAME && exit 0 || exit $?
	;;

  *)
	log_action_msg "Usage: /etc/init.d/$APP_NAME {start|stop|restart|try-restart|status}"
	exit 1
esac

exit 0

