# Samba 遇到的问题
### Samba权限方法，但是客户机无法访问
![alt text](samba_client_problem.png)
- 由于客户机之前访问过，所以Samba会缓存信息，导致客户机无法访问共享目录
    - 解决方法：重新启动Workstation服务，最简单直接，计算机——管理——服务
        ![alt text](samba_workstation.png) 