## Deploying The Challenge

### Requirements
- python3
- [Socat](https://www.redhat.com/sysadmin/getting-started-socat)

```bash
sudo apt install socat
```

### Hosting

Create script to run TCP server on port `5050` and make it executable.
```bash
#run.sh
socat -T60 TCP-LISTEN:5050,reuseaddr,fork EXEC:"python3 -u challenge.py"
```

Create service `sudo vim /etc/systemd/system/decompress_me.service` 

```
[Unit]
Description=decompress_me daaemon
After=network.target

[Service]
User=sa
Group=www-data
WorkingDirectory=/home/sa/interview_problems/decompress_me/resources
ExecStart=/home/sa/interview_problems/decompress_me/resources/run.sh
[Install]
WantedBy=multi-user.target
```

Start and enable service

```
sudo systemctl start decompress_me.service
sudo systemctl enable decompress_me.service
```