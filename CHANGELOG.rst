^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package sick_safetyscanners_base
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1.0.4 (2026-08-13)
------------------
* Added support for missing I/O scanners and EtherCAT type codes
* Setting C++14 as target property
* Fixing warnings for Wpedantic build
* Adding clang-format file and formatting changes
* Contributors: Christian Eichmann, jncfa-kin

1.0.3 (2024-08-20)
------------------
* Adding missing includes for ubuntu24 compiler
* Adding dependencies on specific boost libraries, exporting dep to chrono
* Fix Sync example
* Fix read checksums
* UDPPacketMerger: fixing missing include
* Contributors: Carl Morgan, Denis Taniguchi, Lennart Puck, Marco Bassa, Matthias Schoepfer, Rein Appeldoorn, Soren Holm, 张天宇

1.0.2 (2022-07-19)
------------------
* Added missing 'using' statement to UDPClient.cpp
* Contributors: Lennart Puck, crown-bdee

1.0.1 (2021-12-22)
------------------
* Merge fix for memory leak in command execution
* Fix memory leak in createAndExecuteCommand
* Adding multicast functionality
* Merge constant setting of field angles
* Contributors: Andrew Kooiman, Lennart Puck

1.0.0 (2021-01-11)
------------------

* Initial Release
