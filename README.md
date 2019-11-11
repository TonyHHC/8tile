# 8tile
Chinese 8-Tile Puzzle
使用 BFS 演算法解決 8 tile puzzle

# why?
為了幫同事兒子寫作業，重拾好久未寫的 c 程式

# 開發環境
ubuntu 18.04 + gcc + SDL library

# SDL 的安裝
sudo apt install libsdl2-dev  
可參考 https://www.libsdl.org/

# 8tile_bfs_array.c
使用 array 來建立搜尋樹

# 8tile_bfs_linklist.c
使用 linked list 來建立搜尋樹

# 8tile_bfs_array_sdl.c
基於 8tile_bfs_array.c + SDL 函式庫 用動畫顯示結果，動畫範例為 8tile_bfs_array_sdl.mp4
