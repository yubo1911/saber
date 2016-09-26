#!/bin/bash
echo './py-grep . "(\w)+@(\w)+((\.\w+)+)" --ignorepath ./test_path1' 
./py-grep . "(\w)+@(\w)+((\.\w+)+)" --ignorepath ./test_path1 
echo '===================='
echo './py-grep . "(\w)+@(\w)+((\.\w+)+)" --ignorepath test_path1'
./py-grep . "(\w)+@(\w)+((\.\w+)+)" --ignorepath test_path1 
