import glob, shutil, os

# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

# if not os.path.exists("extras/tmp/ESPert"):
#     os.makedirs("extras/tmp/ESPert")

for file in glob.iglob('src/*.*'):
    print 'Copied %s' % (file)
    shutil.copy2(file, ".")
