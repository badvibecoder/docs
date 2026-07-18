

### Grubby

Update grub entries, for example adding a disable for intel hangcheck for long running gpu kernels: `i915.enable_hangcheck=0`

```bash
# Add
sudo grubby --update-kernel=ALL --args="i915.enable_hangcheck=0"

# Validate
sudo grubby --info=DEFAULT

sudo reboot

# Remove
sudo grubby --update-kernel=ALL --remove-args="i915.enable_hangcheck=0"
```


