echo "working directory:"
read dir					# read a directory address from user

if [ $dir ]					# if an input is given; (if not given, keep working in the current directory)
then
	cd $dir					# move to the given directory
	if [ $? -ne 0 ]			# if the 'cd' command fails
	then					# quit
		echo "Error: cannot access directory"
		exit 0
	fi
fi

for fp in *					# for all files/directories in the working directory
do
	if [ -f $fp ]			# only for files
	then					# change name
		mv $fp `echo $fp | tr '[A-Z][a-z]' '[a-z][A-Z]'`	
	fi
done
