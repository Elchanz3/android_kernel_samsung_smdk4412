#!/bin/sh

TYPE=signal
INTERFACE=org.freedesktop.ConsoleKit.Manager
MEMBER=SystemIdleHintChanged

SUSPEND_CPUFREQ_GOVERNOR=powersave
RESUME_CPUFREQ_GOVERNOR=pegasusq

do_suspend() {
#  cpufreq-set -g $SUSPEND_CPUFREQ_GOVERNOR
#  sleep 1
  echo 'mem' > /sys/power/state
  sleep 1
  cat /sys/power/wait_for_fb_wake
  sleep 1
  echo 'on' > /sys/power/state
#  sleep 2
#  cpufreq-set -g $RESUME_CPUFREQ_GOVERNOR
}

do_wakeup() {
  echo 'on'
}

dbus-monitor --system "type='$TYPE',interface='$INTERFACE',member='$MEMBER'" |
while read key value; do
  if [ "$key" = 'boolean' ]
    then
      case "$value" in
        'true')
          echo 'sleep'

          do_suspend
        ;;
        'false')
          echo 'wakeup'

          do_wakeup
        ;;
      esac
  fi
done

