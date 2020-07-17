Schema is defined in schema5.json. You can define a configuration in a json5
file. (Any json file is also valid json5.)

To encode:
```
$ cp example.json5 test_config.json5
$ ./tool.py test_config.json5 
$ cat test_config.bin 
627;1;433;125;4;0;1;16;112;1;106;4;0;4;38;35;2;30;11259375;305419896;4294905855;4;4;1;50;47;1;42;3735928559;4294950639;140989193;141033471;185;4;1;4;16;142;1;136;4;0;2;126;122;2;116;2517360851987341713937;314824432191309680913;4444580219171430789360;187723572702975;281401388481450;264917625139440;19;24;1;268452384;2;268452392;68;12;0;1;2;1;4;1;17;12;1;8;1;1;2;1;18;29;1;12;1;1;2;1;3;1;2;8;3;1;5;1;41;8;1;1;3;1;17;8;1;4;2;1;18;12;1;4;2;1;3;1;16;98;95;1;4;0;1;2;74;71;2;29;324498737;550528;68702761215;1;32;3615;4095;4276944896;4293783552;3;4;0;5;17;19;16;1;4;0;1;3;4;1;4;18;13;1;8;2;2;3;32;19;34;31;20;2147483648;33554432;2;1;3;1;$ 
```

The encoded data is clearly not compact. It's just a stream of numbers, that we
can make compact by encoding them in binary in a variable-length number format
as is used by e.g. protobuf and MessagePack.

`./tool.py --help` shows more options.
