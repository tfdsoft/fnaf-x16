import sys
import csv
import os
import string

def main():
    def gettile():
        oldtile = your_list[i][j]
        newtile = (int(oldtile)%0x400) # ONLY 1024 TILES PER TILEBASE

        if ((int(oldtile) & 0x80000000)>0): # H-FLIP
            newtile += 0x400
        if ((int(oldtile) & 0x40000000)>0): # V-FLIP
            newtile += 0x800

        return newtile



    for csv_name in sys.argv[1:]:
        print(f"---{csv_name}---")

        with open(f"{csv_name}", 'r') as oldfile:
            reader = csv.reader(oldfile)
            your_list = list(reader)

        newfile = open(f"../{(csv_name.upper())[:-4]}.RLE", 'wb')

        rows = len(your_list)
        columns = len(your_list[0])


        #if(columns != 32):
        #    print(f"Error: {csv_name}.csv is not 32 tiles wide.")
        #    exit()


        run = 0
        
        for i in range (0, rows):
            for j in range (0, columns):
                if (run == 0):
                    value = gettile()
                    run = 1
                else:
                    current_value = gettile()
                    if (current_value == value) and (run < 15):
                        run += 1
                    else:
                        newfile.write((int(value)&0xff).to_bytes())
                        newfile.write((((int(run)<<4)+(int(value)>>8))&0xff).to_bytes())
                        value = current_value
                        run = 1
        newfile.write((int(value)&0xff).to_bytes())
        newfile.write((((int(run)<<4)+(int(value)>>8))&0xff).to_bytes())

        newfile.write(int(0).to_bytes())
        newfile.write(int(0).to_bytes())

        print("complete!")

        oldfile.close
        newfile.close


main()