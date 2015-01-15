from generate_datatypes import *

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--only-needed', action='store_true', help='generate only types needed for compile')
    parser.add_argument('xml', help='path/to/Opc.Ua.Types.bsd')
    parser.add_argument('outfile', help='outfile w/o extension')

    args = parser.parse_args()
    outname = args.outfile.split("/")[-1] 
    inname = args.xml.split("/")[-1]

    fh = open(args.outfile + "_generated.h",'w')
    def printh(string):
        print(string, end='\n', file=fh)

    types = parseTypeDefinitions(args.xml)

    printh('''/**
 * @file ''' + outname + '''_generated.h
 *
 * @brief Autogenerated data types
 *
 * Generated from ''' + inname + ''' with script ''' + sys.argv[0] + '''
 * on host ''' + platform.uname()[1] + ''' by user ''' + getpass.getuser() + ''' at ''' + time.strftime("%Y-%m-%d %I:%M:%S") + '''
 */

#ifndef ''' + outname.upper() + '''_GENERATED_H_
#define ''' + outname.upper() + '''_GENERATED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_types.h"

/**
 * @ingroup types
 *
 * @defgroup ''' + outname + '''_generated Autogenerated ''' + outname + ''' Types
 *
 * @brief Data structures that are autogenerated from an XML-Schema.
 * @{
 */''')

    maxmem = 0
    for t in types.itervalues():
        printh("")
        if type(t) == StructuredType:
            if len(t.members) > maxmem:
                maxmem = len(t.members)
        if t.description != "":
            printh("/** @brief " + t.description + "*/")
        printh(t.string_c())

    print(len(types))
    print(maxmem)
    printh('''
/// @} /* end of group */

#ifdef __cplusplus
} // extern "C"
#endif

#endif''')

    fh.close()

if __name__ == "__main__":
    main()
