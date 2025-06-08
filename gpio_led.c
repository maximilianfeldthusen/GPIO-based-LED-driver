#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/leds.h>

struct gpio_led {
    struct led_classdev  cdev;
    struct gpio_desc    *gpiod;
};

/* called when user writes 0 or 1 to brightness */
static void gpio_led_set(struct led_classdev *cdev,
                         enum led_brightness value)
{
    struct gpio_led *led = container_of(cdev, struct gpio_led, cdev);

    /* drive the GPIO high or low */
    gpiod_set_value(led->gpiod, value ? 1 : 0);
}

static int gpio_led_probe(struct platform_device *pdev)
{
    struct device       *dev = &pdev->dev;
    struct gpio_led     *led;
    int                  ret;

    /* allocate driver state in managed memory */
    led = devm_kzalloc(dev, sizeof(*led), GFP_KERNEL);
    if (!led)
        return -ENOMEM;

    /* grab the “led” GPIO, start low */
    led->gpiod = devm_gpiod_get(dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led->gpiod))
        return dev_err_probe(dev, PTR_ERR(led->gpiod),
                             "failed to get led gpio\n");

    /* populate the LED‐class struct */
    led->cdev.name           = devm_kasprintf(dev, GFP_KERNEL,
                                              "%pOFn", dev->of_node);
    led->cdev.brightness_set = gpio_led_set;
    led->cdev.max_brightness = 1;

    /* register with /sys/class/leds/<name>/ */
    ret = devm_led_classdev_register(dev, &led->cdev);
    if (ret)
        dev_err(dev, "failed to register LED %s\n",
                led->cdev.name);

    dev_info(dev, "registered LED %s on GPIO\n", led->cdev.name);
    return ret;
}

static const struct of_device_id gpio_led_of_match[] = {
    { .compatible = "mycompany,gpio-led" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, gpio_led_of_match);

static struct platform_driver gpio_led_driver = {
    .driver = {
        .name           = "gpio-led",
        .of_match_table = gpio_led_of_match,
    },
    .probe  = gpio_led_probe,
    /* .remove is no-op: devm helpers undo everything */
};
module_platform_driver(gpio_led_driver);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple GPIO-based LED platform driver");
MODULE_LICENSE("GPL");
