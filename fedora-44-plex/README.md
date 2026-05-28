# Quick Setup - Fedora 44 Server - Plex - 64TB (4 x 16TB) JBOD - Ramdisk

No security

## Post install expand the / lv
```bash
sudo lvextend -r -l +100%FREE /dev/fedora/root
```

## Create the ramdisk mount directory
```bash
sudo mkdir -p /mnt/ramdisk
```

## Append the ramdisk configuration to fstab
```bash
echo "tmpfs /mnt/ramdisk tmpfs rw,size=16G,mode=1777,x-systemd.automount 0 0" | sudo tee -a /etc/fstab
```

## Mount the ramdisk immediately
```bash
sudo mount /mnt/ramdisk
```

## Format all 4 drives with ext4
```bash
for disk in sda sdb sdc sdd; do sudo mkfs.ext4 -F /dev/$disk; done
```

## Create mount directories
```bash
sudo mkdir -p /mnt/disk1 /mnt/disk2 /mnt/disk3 /mnt/disk4
```

## Mount the disks temporarily to gather details
```bash
sudo mount /dev/sda /mnt/disk1 && \
sudo mount /dev/sdb /mnt/disk2 && \
sudo mount /dev/sdc /mnt/disk3 && \
sudo mount /dev/sdd /mnt/disk4
```

## Loop through disks and append their UUIDs to fstab
```bash
for i in {1..4}; do
    disk=$(echo "sda sdb sdc sdd" | cut -d' ' -f$i)
    uuid=$(sudo blkid -s UUID -o value /dev/$disk)
    echo "UUID=$uuid /mnt/disk$i etx4 defaults,nofail 0 2" | sudo tee -a /etc/fstab
done
```

## Create folders on each disk
```bash
for i in {1..4}; do
    sudo mkdir -p /mnt/disk$i/tv /mnt/disk$i/movies /mnt/disk$i/torrents /mnt/disk$i/unfinished
done
```

## Set permissions for shared group access
```bash
sudo chmod -R 777 /mnt/disk1 /mnt/disk2 /mnt/disk3 /mnt/disk4
```

## Download and install plex server

## Enable plex service
```bash
sudo systemctl enable --now plexmediaserver
sudo systemctl status plexmediaserver
```

## Disable firewall
```bash
sudo systemctl disable --now firewalld
```

## Install qBittorrent network-only version
```bash
sudo dnf install -y qbittorrent-nox
```

## Create the service configuration directory
```bash
sudo mkdir -p /config/qBittorrent/config/
```

## Write the configuration file to bypass auth and allow all IPs
```bash
sudo tee /config/qBittorrent/config/qBittorrent.conf << 'EOF'
[LegalNotice]
Accepted=true

[Preferences]
WebUI\Address=*
WebUI\AlternativeUIEnabled=false
WebUI\AuthSubnetWhitelist=0.0.0.0/0
WebUI\AuthSubnetWhitelistEnabled=true
WebUI\ClickjackingProtection=false
WebUI\CSRFProtection=false
WebUI\HostHeaderValidation=false
WebUI\LocalHostAuthRequired=false
WebUI\Port=8080
WebUI\ReverseProxyAuthenticationEnabled=false
WebUI\SecureCookie=false
WebUI\UseUPnP=false
EOF
```

## Fix folder permissions for the configuration
```bash
sudo chmod -R 777 /config/qBittorrent
```

## Create Systemd Service File
```bash
sudo tee /etc/systemd/system/qbittorrent.service << 'EOF'
[Unit]
Description=qBittorrent Command Line Client
After=network.target

[Service]
Type=simple
User=root
Environment=QBITTORRENT_HOME=/config
ExecStart=/usr/bin/qbittorrent-nox
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF
```

## Reload systemd, enable and start service
```bash
sudo systemctl daemon-reload
sudo systemctl enable --now qbittorrent
```

## Grab temporary password for first login, admin / temppass
```bash
journalctl -u qbittorrent.service --no-pager | grep "temporary password"
```

## Login and configure new password and whitelists
