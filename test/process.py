import re
import sys

for fn in sys.argv[1:]:
	code = open(fn).read().splitlines()
	new_code = []
	for line in code:
		if line.strip().startswith("cpuinfo_caches l"):
			continue
		if "switch (k)" in line:	
			line = line.replace("switch (k)", "switch (i)")
		elif "ASSERT_EQ(k," in line:
			line = line.replace("ASSERT_EQ(k,", "ASSERT_EQ(i,")
		new_code.append(line)

	open(fn, "w").write("\n".join(new_code))
