# walking_sim
 A small programm, based on SFML graphics, has some capabilities:
 _____mostly done now____________
1) Makes a window where you can place (create objects in main) rooms, construct walls around them and other object, for example, doors.
2) You can move through rooms and be blocked by objects (walls, tables etc.), but can go throug opened (rotated) doors (doorways) or windows
3) The main idea is to check in which room subject (hero or cursor) are, and only then check interactions with objects only in this room (class room has vector of pointers to objects placed in it)
4) distance searching works like this: finds the distance to all borders of object and take minimum (can be muliple minimums - push from all such sides)

______what is planned to be done___________
1) learn how to work with webpages - create an object "tv" that will open a window where you tipe the title of film/anime; program will search on certain websites for this title and wikk return the addres of a page with this movie.
2) implement more types of "furniture" just to train a bit
3) ....
