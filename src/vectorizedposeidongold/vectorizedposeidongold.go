package vectorizedposeidongold

func Hash(in [8]uint64, capacity [4]uint64) ([4]uint64, error) {
	var result [4]uint64 = [4]uint64{0, 0, 0, 0}
	HashWithResult(&in, &capacity, &result)
	return result, nil
}

func HashDirect(in *[8]uint64, capacity *[4]uint64) [4]uint64 {
	var result [4]uint64 = [4]uint64{0, 0, 0, 0}
	HashWithResult(in, capacity, &result)
	return result
}
