Allowed options:  
  --help                produce help message
  --source arg          the source folder to copy from
  --type arg            the file type you want to copy
  --copy-recent arg     if you only want to copy the recent files, the new 
                        files after the last backup
  --misc arg            to enable copy misc folder
  --dest arg            the destination folder to copy to
  --verbose arg         for debug

This program is aiming to copy images from one folder to another.
It is not designed to copy images, but not limit to.
The target file copied will be rename as <Creation Date>_<Original Filename>
Eg. example.jpg  ->>   2017_03_21_example.jpg.
Mandatory Parameters: 
--source 
	The name of the device. eg. EOS_DIGITAL
--type
	Filtype to copy
		 1: only copy JPEG
		 2: only copy CR2
		 3: copy both JPEG and CR2
--copy-recent
	The parameter that specify which files you want to copy from.
		 1: copy files since the last backup
		 2: copy files since the beginning of the time (all files in the device)

Optional Parameters: 
--misc
	Choose whether MISC folder should be copied or not, usually contains movies
		 Default value: 0
		 0: not copy MISC folder
		 1: copy MISC folder
--dest
	Choose what destination folder to backup the images
		 Default value: /Volumes/PHOTO_BACKUP_128G/
		 Pass in an absolute path to specify.

