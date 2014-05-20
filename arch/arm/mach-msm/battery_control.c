/*
 * Author: Willi Ye <williye97@gmail.com>
 *
 * Copyright 2014 Willi Ye
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kallsyms.h>

#include <linux/battery/sec_charger.h>

#include "battery_control.h"

char *battery_cables[] = {
	"Unknown", "Battery", "UPS", "MAINS",
	"USB", "USB_DCP", "USB_CDP", "USB_ACA",
	"MISC", "Cardock", "Wireless", "UartOff",
	"OTG", "BMS", NULL
};

static ssize_t input_limit_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf) {

	int i;
	int value = 0;

	for (i = 0; battery_cables[i] != NULL; i++)
		if (charging_current_table[i].input_current_limit != 0)
			value += sprintf(buf + value, "%s: %d\n", battery_cables[i],
							charging_current_table[i].input_current_limit);

	return value;
}

static ssize_t input_limit_store(struct kobject *kobj, struct kobj_attribute *attr,
				const char *buf, size_t count) {

	int i;
	char cable[16];
	int voltage;

	if (!count)
		return -EINVAL;

	sscanf(buf, "%s %d", &cable[0], &voltage);

	if (voltage >= 460 && voltage <= 1900)
		for (i = 0; battery_cables[i] != NULL; i++)
			if (!strcmp(battery_cables[i], cable) && charging_current_table[i].input_current_limit != 0) {
				pr_debug("set voltage of %s to %d\n", cable, voltage);
				charging_current_table[i].input_current_limit = voltage;
				break;
			}

	return count;
}

static ssize_t fastcharging_current_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf) {

	int i;
	int value = 0;

	for (i = 0; battery_cables[i] != NULL; i++)
		if (charging_current_table[i].fast_charging_current != 0)
			value += sprintf(buf + value, "%s: %d\n", battery_cables[i],
							charging_current_table[i].fast_charging_current);

	return value;
}

static ssize_t fastcharging_current_store(struct kobject *kobj, struct kobj_attribute *attr,
				const char *buf, size_t count) {

	int i;
	char cable[16];
	int voltage;

	if (!count)
		return -EINVAL;

	sscanf(buf, "%s %d", &cable[0], &voltage);

	if (voltage >= 460 && voltage <= 2100)
		for (i = 0; battery_cables[i] != NULL; i++)
			if (!strcmp(battery_cables[i], cable) && charging_current_table[i].fast_charging_current != 0) {
				pr_debug("set voltage of %s to %d\n", cable, voltage);
				charging_current_table[i].fast_charging_current = voltage;
				break;
			}

	return count;
}

static struct kobj_attribute input_limit_attribute =
	__ATTR(input_limit, 0666, input_limit_show, input_limit_store);
static struct kobj_attribute fastcharging_current_attribute =
	__ATTR(fastcharging_current, 0666, fastcharging_current_show, fastcharging_current_store);

static struct attribute *battery_control_attrs[] = {
	&input_limit_attribute.attr,
	&fastcharging_current_attribute.attr,
	NULL,
};

static struct attribute_group battery_control_attr_group = {
	.attrs = battery_control_attrs,
};

static struct kobject *battery_control_kobj;

static int battery_control_init(void) {
	int ret;

	battery_control_kobj =
		kobject_create_and_add("battery_control", kernel_kobj);

	if (!battery_control_kobj) {
		pr_err("%s battery_control_kobj create failed!\n", __FUNCTION__);
		return -ENOMEM;
        }

	ret = sysfs_create_group(battery_control_kobj,
			&battery_control_attr_group);

	if (ret) {
		pr_info("%s sysfs create failed!\n", __FUNCTION__);
		kobject_put(battery_control_kobj);
	}

	return ret;
}

static void battery_control_exit(void) {
	if (battery_control_kobj != NULL)
		kobject_put(battery_control_kobj);
}

module_init(battery_control_init);
module_exit(battery_control_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Willi Ye <williye97@gmail.com>");
MODULE_DESCRIPTION("Battery Control Module GPL Edition");
