SelfImage version 1.2.1 build 92 (1.2.1.92)

About SelfImage
---------------

SelfImage is the little hard drive utility with big aspirations.

In its current incarnation, SelfImage is capable of making an image of a hard
disk partition and writing it to another - useful for making backups.  Unlike
dd for Windows (or cygwin), SelfImage is capable of creating an image of a
partition that is currently in use.

Additionally, when run on Windows 2000 or XP, SelfImage can create images of
partitions that Windows doesn't have mounted on a drive letter.  Perfect for
the dual-boot system, you can create an image backup of a Linux partition
directly from Windows.

From version 0.2 on, SelfImage was capable of writing an image file back to a
partition, though with some limitations.  The limitation being that you can
only write to partitions that either aren't mounted, or which can be locked
for writing.  Windows cannot lock a partition that has open files, so this
means that you can't restore to, for example, your boot partition.  This must
still be done outside Windows (with Linux, for example).

SelfImage can also copy one partition to another, or even one file to another
(though it would have to be a very large file to bother running SelfImage to
copy it).

As of version 1.0, SelfImage is capable of on-the-fly compression and
decompression of images.  SelfImage can perform the compression in parallel,
to take advantage of hyperthreading, multi-core CPUs, and multi-CPU systems.

Version 1.1 added the ability for SelfImage to determine what portions of a
local disk are free space and to skip reading them.  This improves the
processing of compressed images in two ways: speed is improved as SelfImage
replaces the skipped portiosn with precompressed "zeros".  Compression is
improved, as SelfImage doesn't have to try to compress the leftovers of old,
deleted files.  Also new in 1.1 was support for taking images of remote
partitions using the Network Block Device protocol.

Since version 1.2, SelfImage's ability to skip unallocated space has been
extended to Linux ext2/ext3 partitions.  This is possible both for local and
remote (Network Block Device) partitions.
