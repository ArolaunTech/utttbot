from copy import deepcopy

def evaluate(board):
	bigxs = 0
	bigos = 0
	spaces = 0
	for i in range(4):
		xs = 0
		os = 0
		for j in range(4):
			cell = board[0][4 * i + j]

			if cell == 1:
				xs += 1
			if cell == -1:
				os += 1
			if cell == 0:
				spaces += 1
		if xs >= 2:
			bigxs += 1
		if os >= 2:
			bigos += 1
	if bigxs >= 2:
		return 1
	if bigos >= 2:
		return -1
	if spaces > 0:
		return 2
	return 0

def search(board):
	evaluation = evaluate(board)
	if evaluation != 2:
		return evaluation

	playboard = board[1]
	player = board[2]

	best = -player

	if playboard == 4:
		for i in range(16):
			if board[0][i] != 0:
				continue

			xs = 0
			os = 0
			for j in range(4):
				cell = board[0][4 * (i // 4) + j]

				if cell == 1:
					xs += 1
				if cell == -1:
					os += 1
			if xs >= 2:
				continue
			if os >= 2:
				continue
			if (xs + os) == 4:
				continue

			newboard = deepcopy(board)

			newboard[0][i] = player
			newboard[2] = -player

			newboard[1] = i % 4

			xs = 0
			os = 0
			for j in range(4):
				cell = newboard[0][4 * newboard[1] + j]

				if cell == 1:
					xs += 1
				if cell == -1:
					os += 1
			if xs >= 2:
				newboard[1] = 4
			if os >= 2:
				newboard[1] = 4
			if (xs + os) == 4:
				newboard[1] = 4

			neweval = search(newboard)

			if neweval * player > best * player:
				best = neweval

			if best == player:
				break
	else:
		for i in range(4):
			if board[0][i + 4 * playboard] != 0:
				continue

			newboard = deepcopy(board)

			newboard[0][i + 4 * playboard] = player
			newboard[2] = -player

			newboard[1] = i

			xs = 0
			os = 0
			for j in range(4):
				cell = newboard[0][4 * newboard[1] + j]

				if cell == 1:
					xs += 1
				if cell == -1:
					os += 1
			if xs >= 2:
				newboard[1] = 4
			if os >= 2:
				newboard[1] = 4
			if (xs + os) == 4:
				newboard[1] = 4

			neweval = search(newboard)

			if neweval * player > best * player:
				best = neweval

			if best == player:
				break

	moves = sum(list(map(abs, board[0])))
	if (best != -1):
		print(moves, board[0], playboard, player)

	return best


print(
	search(
		[[0 for i in range(16)], 4, 1]
	)
)