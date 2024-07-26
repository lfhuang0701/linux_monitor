#!/usr/bin/env bash

<<<<<<< HEAD

# 这个脚本主要用于在重新启动 linux_monitor Docker容器之前，停止并删除现有的同名容器。
# 它还设置了多个环境变量和挂载路径，以确保容器内能够访问和使用主机上的资源。
# 1.获取本地挂载路径 2.设置display显示变量，使得容器内图形界面能够显示到主机 3.获取用户和系统信息 4. 停止和删除现有同名容器
# 5.启动新的容器，其中传递环境变量（包括获取的用户和系统信息，目的在于确保容器能够正确配置和运行），设定容器权限，挂载主机目录到容器，并使用主机网络

# ${BASH_SOURCE[0]}  = /home/roster/Desktop/linux_monitor/work/private-node/docker/scripts/m          onitor_docker_run.sh
# dirname "${BASH_SOURCE[0]}" = /home/roster/Desktop/linux_monitor/work/private-node/docker/scripts/
# cd "$( dirname "${BASH_SOURCE[0]}" )/../.." = /home/roster/Desktop/linux_monitor/work/private-node/
# "$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )" = /home/roster/Desktop/linux_monitor/work/private-node
# 此行代码主要作用是获取本地目录
# 后续将 MONITOR_HOME_DIR即本地目录挂载到docker内的/work/目录
=======
>>>>>>> e67a9ea38e015b6b5027e5b629c7a9762722496d
MONITOR_HOME_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"

display=""
if [ -z ${DISPLAY} ];then
    display=":1"
else
    display="${DISPLAY}"
fi

local_host="$(hostname)"
user="${USER}"
uid="$(id -u)"
group="$(id -g -n)"
gid="$(id -g)"


echo "stop and rm docker" 
docker stop linux_monitor > /dev/null
docker rm -v -f linux_monitor > /dev/null

echo "start docker"
docker run -it -d \
--name linux_monitor \
-e DISPLAY=$display \
--privileged=true \
-e DOCKER_USER="${user}" \
-e USER="${user}" \
-e DOCKER_USER_ID="${uid}" \
-e DOCKER_GRP="${group}" \
-e DOCKER_GRP_ID="${gid}" \
-e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
-v ${MONITOR_HOME_DIR}:/work \
-v ${XDG_RUNTIME_DIR}:${XDG_RUNTIME_DIR} \
--net host \
linux:monitor