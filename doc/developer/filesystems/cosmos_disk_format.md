# CosmOS Disk Format

Revision 1 - 13 September 2020

All data is little-endian. Blocks are 512 bytes in length; if the block size of the underlying storage medium diverges like that, implementations will need to be prepared to convert. Block addresses are pointers to 512-byte blocks, such that block address 0 points to byte 0, block address 1 points to byte 512, block address 2 to byte 1024, and so forth.

## CONCEPTS:

### 1. Data Space

A data space is the atomic unit of storage in the CosmOS Disk Format. These are the on-disk structures that hold user data, and are designed to be transcluded within multiple presentations. This enables not just reuse of data (space efficiency is NOT a design goal here, and in fact
the fragmentation means that this will probably be less space-efficient than a traditional filesystem except in extreme cases of repetitive
transclusion), but also (more importantly) automatic updating of shared contents between multiple presentations.

For example, if a particular data space contains a person's phone number, then if that person's phone number changes only the data space itself has to be updated, and all presentations that transclude the data space will reflect the change. Data spaces can also be forked, which is useful e.g. if a data space's content changes but it is also important to keep the original content accessible for transclusion.

### 2. Presentation

A presentation is the immediate user-visible data unit. It is most analogous to "documents" in a traditional file system, except it does
not actually store any data itself. Instead, it consists of a set of references to data spaces, which may include data spaces for shared
data, data spaces for formatting information, or data spaces that describe the relationship among the other data spaces in the
presentation. There is no provision for including any data other than data-space references within a presentation.

### 3. Group

A group is a means of organizing presentations and data spaces. A group can contain any combination of one or more presentations, data
spaces, and groups. There is no format-enforced hierarchy: a given data space or presentation can be included in multiple groups, and a
group itself can also be included in multiple groups. Circular groups are also allowed: Group A can contain Group B, which can then be
modified to include Group A.

<pre>
SUPERBLOCK - at block 1, block 1,000,001, block 2,000,001, etc.
	qword #		Description
	0		Magic word = 0x303244594C4F4647
	1		Last mount time (in seconds since midnight UTC January 
			1, 1900 - NTP epoch)
	3		location of primary data space directory (LBA block)
	4		location of primary presentation directory (LBA block)
	5		location of primary group directory (LBA block)
	6-63		reserved for future expansion
	
DATA SPACE/PRESENTATION/GROUP DIRECTORY
	qword #		Description
	0		Magic word = 0x30324D4C424D4C42 for data space 
			directory, 0x4E45444942454F4A for presentation 
			directory, 0x535241484E444942 for group directory)
	1		address of previous data space/presentation/group 
			directory (0 if this is the first directory)
	2		address of next data space/presentation/group directory 
			(0 if this is the last directory)
	3		address of first data space/presentation/group pointed 
			to in directory
	4		address of second data space/presentation/group pointed 
			to in directory, or 0 if no further entries
	.
	.
	.
	63		address of 61st data space/presentation/group pointed 
			to in directory, or 0 if no further entries
</pre>
