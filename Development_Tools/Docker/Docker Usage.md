# Docker 镜像和容器
## 关于Docker
Docker 是一个开源的应用容器引擎，可以轻松的为任何应用创建一个轻量级的、可移植的、自给自足的容器。开发者在本地编译测试通过的容器可以批量地在生产环境中部署，包括VMs（虚拟机）、bare metal、OpenStack 集群和其他的基础应用平台
简单的理解，Docker类似于集装箱，各式各样的货物，经过集装箱的标准化进行托管，而集装箱和集装箱之间没有影响。也就是说，Docker平台就是一个软件集装箱化平台，这就意味着我们自己可以构建应用程序，将其依赖关系一起打包到一个容器中，然后这容器就很容易运送到其他的机器上进行运行，而且非常易于装载、复制、移除，非常适合软件弹性架构
## Docker 基本概念
1. Docker镜像
    Docker镜像是一个只读模板，包含了运行应用程序所需的文件系统结构、依赖、配置等。它由一系列层（layer）组成，每一层代表Dockerfile中的一条指令
2. 容器
    容器是镜像的运行实例。它包含了一个独立的用户空间，以及运行中的应用程序进程。容器使用宿主机的内核，但通过命名空间和控制组（cgroups）等技术提供隔离
3. 镜像仓库
    镜像仓库是一个集中的存储、分发镜像的服务
## Docker 安装
1. 系统检查
    ```bash
    uname -a 
    lsb_release -a
    ```
2. 移除旧环境、更新环境、安装依赖
    ```bash 
    sudo apt-get remove docker docker-engine docker.io containerd runc
    sudo apt update
    sudo apt install -y ca-certificates curl gnupg lsb-release
    ```
3. 配置官方/国内仓库源
    - 信任Docker GPG 密钥
    ```bash
    sudo install -m 0755 -d /etc/apt/keyrings
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
    suudo chmod a+r /etc/apt/keyrings/docker.gpg
    ```
    - 设置稳定版仓库
    ```bash 
    echo \
      "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
      $(. /etc/os-release; echo "$VERSION_CODENAME") stable" | \
      sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
    ```
4. 安装 Docker 引擎
    ```bash
    sudo apt-get update
    sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
    ```
5. 启动，自启和重载
    ```bash
    # 自动
    sudo systemctl start docker
    # 自启
    sudo systemctl enable docker
    # 重启
    sudo systemctl daemon-reload
    sudo systemctl restart docker
    ```
6. 非root使用 Docker
    ```bash
    sudo usermod -aG docker $USER
    ```

## Docker 基本命令
### 基本信息
- 查看存储驱动、内核参数等
    ```bash
    docker info
    ```
- 显示版本信息
    ```bash
    docker version
    ```
### 镜像管理
- 构建镜像
    ```bash
    docker build -t <repo>:<tag> -f Dockerfile .
    ```
    - `<repo>`： 镜像仓库名称
    - `<tag>`： 镜像标签
    - `-t`：指定镜像名称和标签
    - `-f`：指定Dockerfile文件
    - `.`：指定Dockerfile所在目录
**Dockerfile文件可以自己构建也可以用原厂官方获取**
- 拉取镜像
    ```bash
    docker pull <repo>:<tag>
    ```
- 镜像推送
    ```bash
    docker push <repo>:<tag>
    ```
- 导入镜像
    ```bash
    docker load -i image.tar
    ```
- 导出镜像
    ```bash
    docker save -o image.tar <repo>:<tag>
    ```
- 镜像列表
    ```bash
    docker images
    ```
- 镜像重命名
    ```bash 
    docker tag <old_name> <new_name>
    ```
- 删除镜像
    ```bash
    docker rmi <repo>:<tag>
    ```
### 容器管理
- 运行容器
    ```bash
    docker run <options> <repo>:<tag> <command> <args>
    ```
    - `<options>`： 运行选项
        ```
        常用选项
        名称：--name <container_name>
        交互：-it（分配 TTY，保持 STDIN）
        后台：-d（守护进程方式运行）
        自动清理：--rm（容器退出即删除）
        端口映射：-p 宿主端口:容器端口；多端口重复写
        挂载目录：-v 宿主路径:容器路径[:ro]；临时内存盘 --tmpfs /run
        环境变量：-e KEY=VAL；或 --env-file env.list
        工作目录：-w /path/in/container
        资源限制：--memory 512m --cpus 1.0 --pids-limit 128
        权限/设备：
        特权：--privileged（慎用）
        增加能力：--cap-add NET_ADMIN 等
        挂载设备：--device /dev/video0:/dev/video0
        网络：--network <net>；主机网络 --network host
        平台（交叉）：--platform linux/arm64（镜像支持时）
        共享内存：--shm-size 1g（深度学习/浏览器常用）名称：--name <container_name>
        交互：-it（分配 TTY，保持 STDIN）
        后台：-d（守护进程方式运行）
        自动清理：--rm（容器退出即删除）
        端口映射：-p 宿主端口:容器端口；多端口重复写
        挂载目录：-v 宿主路径:容器路径[:ro]；临时内存盘 --tmpfs /run
        环境变量：-e KEY=VAL；或 --env-file env.list
        工作目录：-w /path/in/container
        资源限制：--memory 512m --cpus 1.0 --pids-limit 128
        权限/设备：
        特权：--privileged（慎用）
        增加能力：--cap-add NET_ADMIN 等
        挂载设备：--device /dev/video0:/dev/video0
        网络：--network <net>；主机网络 --network host
        平台（交叉）：--platform linux/arm64（镜像支持时）
        共享内存：--shm-size 1g（深度学习/浏览器常用）
        ```
    - `<repo>:<tag>`： 镜像名称和标签
    - `<command> <args>`： 运行命令
    ```bash
    # example
    docker run -it --name bm1684x_v23_09 \
    -v /home/ubuntu/workspace/Sophgo/bm1684/bm1684x_v23_09:/bm1684x_v23_09 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    --network host \
    --privileged \
    sophon_bsp_build:v1 /bin/bash 
    ```

- 查看容器
    ```bash
    docker ps -a
    ```
- 启动容器
    ```bash
    docker start <container_id>
    ```
- 进入容器
    ```bash
    docker exec -it <container_id> bash
    ```
- 停止容器
    ```bash
    docker stop <container_id>
    ```
- 删除容器
    ```bash
    docker rm <container_id>
    ```
