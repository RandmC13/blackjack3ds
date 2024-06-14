lines = []
with open("cardsheet.t3s", "r") as f:
    for line in f:
        lines.append(line.strip("\n").replace("cards/", ""))
headerline = lines[0]
lines.pop(0)
order = ["S", "C", "D", "H"]
deck = [[], [], [], []]
for card in lines:
    for i in range(len(order)):
        if card[1] == order[i]:
            deck[i].append(card)
order = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"]
sorteddeck = []
for group in deck:
    sorted = []
    for rank in order:
        for card in group:
            if card[0] == rank:
                sorted.append(card)
    sorteddeck.append(sorted)

filelist = [headerline + "\n"]
for i in sorteddeck:
    for card in i:
        filelist.append("cards/" + card + "\n")

with open("cardsheet2.t3s", "w") as f:
    f.writelines(filelist)
