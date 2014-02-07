#!/sbin/busybox sh

cat << CTAG
{
    name:RomSwitcher,
    elements:[
	{ STitleBar:{
		title:"RomSwitcher Settings"
	}},
`
for ROM in 1 2 3 4 5 ; do
	echo '{ SButton:{
                label:"Reboot to ROM '${ROM}'",
                action:"rs reboot '$(($ROM-1))'"
        }},'
done
`
	{ SButton:{
                label:"Install RomSwitcher Recovery",
                action:"rs recovery"
        }},
    ]
}
CTAG
