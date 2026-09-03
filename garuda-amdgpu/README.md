## Garuda AMD GPU/ROCm Install

### Install Basic Setup

```bash
sudo pacman -Syu git openssh lact yay rclone vim curl wget steam obs-studio luanti s-tui

yay -S google-chrome visual-studio-code-bin balena-etcher

# Install uv, ollama
curl -LsSf https://astral.sh/uv/install.sh | sh
curl -fsSL https://ollama.com/install.sh | sh

# Add option that will unlock RDNA2 (6950 XT) memory controller, otherwise it may fallback to 97mhz
sudo vim /etc/default/grub
# Add this
GRUB_CMDLINE_LINUX_DEFAULT="... amdgpu.dcdebugmask=0x10"
# Update grab
sudo update-grub
sudo reboot

# Validate mem clocks in game with this
cat /sys/class/drm/card*/device/pp_dpm_mclk
# Validate rebar/sam with this
cat /sys/class/drm/card*/device/mem_info_vis_vram_total

```

### Update Grub for LACT

```bash
sudo vim /etc/default/grub

# Add featuremask to GRUB_CMDLINE_LINUX_DEFAULT
amdgpu.ppfeaturemask=0xffffffff
# Save and exit
sudo update-grub
sudo reboot
```

### Install ROCm

```bash
sudo pacman -Syu
sudo usermod -aG render,video $USER
sudo pacman -S rocm-hip-sdk rocm-opencl-sdk

echo " " >> ~/.config/fish/config.fish
echo " " >> ~/.config/fish/config.fish
echo "set -gx ROCM_PATH=/opt/rocm" >> ~/.config/fish/config.fish
echo " " >> ~/.config/fish/config.fish
echo "set -gx HSA_OVERRIDE_GFX_VERSION 11.0.0" >> ~/.config/fish/config.fish

sudo reboot
```

### Setup OneDrive

```bash
mkdir ~/OneDrive
rclone config
# Step through the menus

# Create rclone service file
sudo vim /etc/systemd/system/onedrive.service

# Add
[Unit]
Description=OneDrive over rclone Daemon
After=network-online.target
Wants=network-online.target

[Service]
User=pcarroll
Type=simple
ExecStart=/usr/bin/rclone --vfs-cache-mode writes mount OneDrive: /home/pcarroll/OneDrive/ --config /home/pcarroll/.config/rclone/rclone.conf
ExecReload=/bin/kill -s HUP $MAINPID
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
#######

# Enable and start service
sudo systemctl daemon-reload && sudo systemctl enable onedrive.service && sudo systemctl start onedrive.service && sudo systemctl status onedrive.service
```
