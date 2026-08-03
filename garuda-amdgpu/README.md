## Garuda AMD GPU/ROCm Install

Install Basic Setup

```bash
sudo pacman -Syu git openssh lact yay rclone vim curl wget steam obs-studio luanti

yay -S google-chrome visual-studio-code-bin

# Install uv, ollama
curl -LsSf https://astral.sh/uv/install.sh | sh
curl -fsSL https://ollama.com/install.sh | sh
```

Install MESA Driver version of Garuda

```bash
sudo pacman -S --needed linux-zen-headers rocm-hip-sdk rocm-opencl-runtime
sudo gpasswd -a $USER render
sudo gpasswd -a $USER video
# 7900 XTX
echo "set -gx HSA_OVERRIDE_GFX_VERSION 11.0.0" >> ~/.config/fish/config.fish
sudo reboot
```

Setup OneDrive

```bash
mkdir ~/OneDrive
rclone config
```
