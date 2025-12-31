#!/bin/bash

# Docker Image Building and Container Management Script

IMAGE_NAME="sophgo_bm1684x_bsp_build"
DEFAULT_CONTAINER_NAME="bm1684x_default"
DOCKERFILE_PATH="./dockerfile"

show_help() {
    echo "Usage: $0 [option] [container_name]"
    echo ""
    echo "Options:"
    echo "  build               Build Docker image"
    echo "  remove              Remove Docker image"
    echo "  prune               Remove dangling images"
    echo "  run [name]          Start and enter container (default or specified name)"
    echo "  enter [name]        Enter running container (default or specified name)"
    echo "  stop [name]         Stop container (default or specified name)"
    echo "  list                List all containers based on this image"
    echo "  clean               Clean stopped containers"
    echo "  force-clean         Force remove all containers and images related to this project"
    echo "  status              Show container and image status"
    echo "  help                Show help information"
    echo ""
    echo "Examples:"
    echo "  ./bm1684x_docker_manager.sh run                     # Run default container"
    echo "  ./bm1684x_docker_manager.sh run product_a           # Run container named product_a"
    echo "  ./bm1684x_docker_manager.sh enter product_a         # Enter product_a container"
    echo ""
}

build_image() {
    echo "Building Docker image..."
    
    # Check if dockerfile exists (both cases)
    if [ -f "./dockerfile" ]; then
        DOCKERFILE_PATH="./dockerfile"
    elif [ -f "./Dockerfile" ]; then
        DOCKERFILE_PATH="./Dockerfile"
    else
        echo "Error: Dockerfile not found in current directory"
        echo "Expected either 'dockerfile' or 'Dockerfile'"
        exit 1
    fi
    
    echo "Using Dockerfile: $DOCKERFILE_PATH"
    
    # Build with the correct context and Dockerfile
    sudo docker build -t $IMAGE_NAME -f $DOCKERFILE_PATH .
    if [ $? -eq 0 ]; then
        echo "Image built successfully!"
    else
        echo "Failed to build image!"
        exit 1
    fi
}

remove_image() {
    echo "Removing Docker image..."
    if [ "$(sudo docker images -q $IMAGE_NAME 2>/dev/null)" ]; then
        sudo docker rmi -f $IMAGE_NAME
        echo "Image removed successfully!"
    else
        echo "Warning: Image $IMAGE_NAME does not exist"
    fi
}

prune_images() {
    echo "Removing dangling images..."
    dangling_images=$(sudo docker images -f "dangling=true" -q)
    if [ ! -z "$dangling_images" ]; then
        sudo docker rmi -f $dangling_images 2>/dev/null
        if [ $? -eq 0 ]; then
            echo "Dangling images removed."
        else
            echo "Some dangling images could not be removed (possibly in use by containers)."
        fi
    else
        echo "No dangling images found."
    fi
}

run_container() {
    local container_name=${1:-$DEFAULT_CONTAINER_NAME}
    echo "Starting and entering container '$container_name'..."
    
    # Check if container with same name is already running
    if [ "$(sudo docker ps -q -f name=$container_name)" ]; then
        echo "Container $container_name is already running"
        echo "Entering container..."
        sudo docker exec -it $container_name /bin/bash
        return
    fi
    
    # Check if stopped container with same name exists
    if [ "$(sudo docker ps -aq -f status=exited -f name=$container_name)" ]; then
        echo "Found existing container $container_name, starting it..."
        sudo docker start $container_name
        sudo docker exec -it $container_name /bin/bash
        return
    fi
    
    # Check if image exists
    if [ -z "$(sudo docker images -q $IMAGE_NAME 2>/dev/null)" ]; then
        echo "Image $IMAGE_NAME not found. Please build it first with './docker_manager.sh build'"
        exit 1
    fi
    
    # Create and start new container
    echo "Creating new container '$container_name'..."
    sudo docker run -it --name $container_name \
        --privileged=true \
        --shm-size=1g \
        -v $(pwd):/workspace \
        $IMAGE_NAME \
        /bin/bash
        
    echo "Container '$container_name' has exited."
    read -p "Keep this container? (y/n, default y): " keep_container
    keep_container=${keep_container:-y}
    
    if [[ $keep_container =~ ^[Nn]$ ]]; then
        echo "Removing container..."
        sudo docker rm $container_name
        echo "Container removed."
    fi
}

enter_container() {
    local container_name=${1:-$DEFAULT_CONTAINER_NAME}
    echo "Attempting to enter running container '$container_name'..."
    if [ "$(sudo docker ps -q -f name=$container_name)" ]; then
        sudo docker exec -it $container_name /bin/bash
    else
        echo "No running container named $container_name found"
        echo "Start it? (y/n)"
        read start_container
        if [[ $start_container =~ ^[Yy]$ ]]; then
            if [ "$(sudo docker ps -aq -f name=$container_name)" ]; then
                sudo docker start $container_name
                sudo docker exec -it $container_name /bin/bash
            else
                echo "No container $container_name found, please use 'run' command first"
            fi
        fi
    fi
}

stop_container() {
    local container_name=${1:-$DEFAULT_CONTAINER_NAME}
    echo "Stopping container '$container_name'..."
    if [ "$(sudo docker ps -q -f name=$container_name)" ]; then
        sudo docker stop $container_name
        echo "Container stopped."
    else
        echo "Container '$container_name' is not running."
    fi
}

list_containers() {
    echo "=== Containers based on image '$IMAGE_NAME' ==="
    sudo docker ps -a --format "table {{.Names}}\t{{.Status}}\t{{.CreatedAt}}" -f ancestor=$IMAGE_NAME
}

clean_containers() {
    echo "Cleaning stopped containers..."
    stopped_containers=$(sudo docker ps -aq -f status=exited -f ancestor=$IMAGE_NAME)
    if [ ! -z "$stopped_containers" ]; then
        sudo docker rm $stopped_containers
        echo "Stopped containers cleaned."
    else
        echo "No stopped containers to clean."
    fi
}

force_clean() {
    echo "Force removing all containers and images related to this project..."
    
    # Stop all running containers with our image
    running_containers=$(sudo docker ps -q -f ancestor=$IMAGE_NAME)
    if [ ! -z "$running_containers" ]; then
        echo "Stopping containers based on $IMAGE_NAME..."
        sudo docker stop $running_containers 2>/dev/null || true
    fi
    
    # Remove all containers based on our image
    all_containers=$(sudo docker ps -aq -f ancestor=$IMAGE_NAME)
    if [ ! -z "$all_containers" ]; then
        echo "Removing containers based on $IMAGE_NAME..."
        sudo docker rm -f $all_containers 2>/dev/null || true
    fi
    
    # Remove our specific image
    if [ "$(sudo docker images -q $IMAGE_NAME 2>/dev/null)" ]; then
        echo "Removing image $IMAGE_NAME..."
        sudo docker rmi -f $IMAGE_NAME 2>/dev/null || true
    fi
    
    # Prune dangling images
    echo "Pruning dangling images..."
    sudo docker image prune -f 2>/dev/null || true
    
    # System prune to clean everything
    echo "Running system prune to clean up everything..."
    sudo docker system prune -f 2>/dev/null || true
    
    echo "Force clean completed."
}

status_check() {
    echo "=== Docker Images ==="
    sudo docker images | grep -E "(REPOSITORY|$IMAGE_NAME|^<none>)"
    
    echo ""
    echo "=== Running Containers ==="
    sudo docker ps -f ancestor=$IMAGE_NAME | grep -E "(CONTAINER|$IMAGE_NAME)"
    
    echo ""
    echo "=== Stopped Containers ==="
    sudo docker ps -a -f ancestor=$IMAGE_NAME | grep Exited
    
    echo ""
    echo "=== Dangling Images ==="
    dangling_count=$(sudo docker images -f "dangling=true" -q | wc -l)
    echo "Number of dangling images: $dangling_count"
}

case "$1" in
    build)
        build_image
        ;;
    remove)
        remove_image
        ;;
    prune)
        prune_images
        ;;
    run)
        run_container "$2"
        ;;
    enter)
        enter_container "$2"
        ;;
    stop)
        stop_container "$2"
        ;;
    list)
        list_containers
        ;;
    clean)
        clean_containers
        ;;
    force-clean)
        force_clean
        ;;
    status)
        status_check
        ;;
    help|"")
        show_help
        ;;
    *)
        echo "Unknown option: $1"
        show_help
        exit 1
        ;;
esac