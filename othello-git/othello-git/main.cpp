//
//  main.cpp
//  othello
//
//  Created by 李呂元 on 2020/6/25.
//  Copyright © 2020 李呂元. All rights reserved.
//

//
//  main.cpp
//  project edit
//
//  Created by 李呂元 on 2020/6/24.
//  Copyright © 2020 李呂元. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>
using namespace std;
struct Point {
    int x, y;
    Point(int x,int y):x(x),y(y){};
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
    bool operator<(const Point& other) const;
};

bool Point::operator<(const Point& other) const {
    return true;
}

set<Point> direction={Point(0,1),Point(0,-1),Point(1,0),Point(-1,0),Point(1,1),Point(-1,-1),Point(-1,1),Point(1,-1)};

void getvalidpoint(int (*m)[10],int player,vector<Point> valid){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(player==1){          //black
                if(m[i][j]==1){
                    for(auto dir:direction){
                        int xp=dir.x+i;
                        int yp=dir.y+j;
                        if( xp>=0 && xp<=7 && yp>=0 && yp<=7 ){
                            if(m[xp][yp]==2){
                                while(1){
                                    xp+=dir.x;
                                    yp+=dir.y;
                                    if( xp<0 || xp>7 || yp<0 || yp> 7) break;
                                    if(m[xp][yp]==0){
                                        valid.push_back(Point(xp,yp));
                                        break;
                                    }
                                }
                                
                            }
                        }
                        
                    }
                }
            }
            else{                   //white
                if(m[i][j]==2){
                    for(auto dir:direction){
                        int xp=dir.x+i;
                        int yp=dir.y+j;
                        if( xp>=0 && xp<=7 && yp>=0 && yp<=7 ){
                            if(m[xp][yp]==1){
                                while(1){
                                    xp+=dir.x;
                                    yp+=dir.y;
                                    if( xp<0 || xp>7 || yp<0 || yp> 7) break;
                                    if(m[xp][yp]==0){
                                        valid.push_back(Point(xp,yp));
                                        break;
                                    }
                                }
                                
                            }
                        }
                        
                    }
                }
            }
        }
    }
}
class state{
public:
    int val;
    int player;
    int x;
    int y;
    int max;
    int mini;
    int depth;
    int storevalue;    //考慮了當下狀況的走法
    void setvalue();   //存給val
    int map[10][10];
    vector<Point> validspot;
    state(Point a,int play,int (*m)[10],int dep){   //play=the one create the table
        x=a.x;
        y=a.y;
        max=-88888888;
        mini=8888888;
        depth=dep;
        val=-1;
        //player=play;
        for(int i=0;i<8;i++)
            for(int k=0;k<8;k++){
                if( x==i && y==k )
                    map[i][k]=play;
                else
                    map[i][k]=m[i][k];
            }
        int spot= play==1? 2:1;
        for(auto dir:direction){
            vector<Point> tobeflipped;
            int xp=x+dir.x;
            int yp=y+dir.y;
            if( xp <0 || xp>7 || yp<0 || yp>7 ) continue;
            while( xp>=0 && xp<=7 && yp>=0 && yp<=7 ){
                if(map[xp][yp]!=spot) break;
                tobeflipped.push_back(Point(xp,yp));
                xp+=dir.x;
                yp+=dir.y;
            }
            if( xp>=0 && xp<=7 && yp>=0 && yp<=7 ){
                if(map[xp][yp]!=0){
                    while(!tobeflipped.empty()){
                        int x=tobeflipped.begin()->x;
                        int y=tobeflipped.begin()->y;
                        map[x][y]=play;
                        tobeflipped.erase(tobeflipped.begin());
                    }
                }
            }
        }
        if(play==1)
            player=2;  //white
        else player=1; //black
        getvalidpoint(map, player, validspot);
    }
};
#define corner 2000
#define x_square -1500
#define c_square -1250
#define  side 30
void state::setvalue(){
    
    int huristic=0;
    /* if( ( x==0&&y==0 )||( x==0&&y==7 )||( x==7&&y==7 )||( x==7&&y==0 ) ) val=corner;
     
     else  if( ( x==1&&y==1 )||( x==1&&y==6 )||( x==6&&y==6 )||( x==6&&y==1 ))  val=x_square;
     
     else if( ( x==0&&y==1 )||( x==1&&y==0 )||( x==1&&y==7 )||( x==0&&y==6 )||( x==6&&y==0 )||( x==7&&y==1 )||( x==6&&y==7 )||( x==7&&y==6) ) val=c_square;
     
     else{
     int cnt=0,cnttotal=0;
     for(auto dir:direction){
     
     if(player==1){
     int cntx=x+dir.x;
     int cnty=y+dir.y;
     if( cntx>7 || cntx<0 || cnty>7 || cnty<0 ) continue;
     while(map[cntx][cnty]==2){
     cntx+=dir.x;
     cnty+=dir.y;
     if( cntx>7 || cntx<0 || cnty>7 || cnty<0 || map[cntx][cnty]==1 ) break;
     if( map[cntx][cnty]==0 ){
     cnt=0;
     }
     cnt++;
     }
     }
     else if(player==2){
     int cntx=x+dir.x;
     int cnty=y+dir.y;
     if( cntx>7 || cntx<0 || cnty>7 || cnty<0 ) continue;
     while(map[cntx][cnty]==1){
     cntx+=dir.x;
     cnty+=dir.y;
     if( cntx>7 || cntx<0 || cnty>7 || cnty<0 || map[cntx][cnty]==1 ) break;
     if( map[cntx][cnty]==0 ){
     cnt=0;
     }
     cnt++;
     }
     }
     cnttotal+=cnt;
     }
     val=10*cnt;
     }*/
    int pp= player==1? 2:1;
    
    if(map[0][0]==pp) huristic+=corner;
    else if(map[0][0]==0){
        if(map[0][1]==pp || map[1][0]==pp) huristic+=c_square;
        if(map[1][1]==pp) huristic+=x_square;
    }
    else{
        huristic-=corner;
    }
    
    if(map[7][0]==pp) huristic+=corner;
    else if(map[7][0]==0){
        if(map[7][1]==pp || map[6][0]==pp) huristic+=c_square;
        if(map[6][1]==pp) huristic+=x_square;
        
    }
    else{
        huristic-=corner;
    }
    
    if(map[7][7]==pp) huristic+=corner;
    else if(map[7][0]==0){
        if(map[7][6]==pp || map[6][7]==pp) huristic+=c_square;
        if(map[6][6]==pp) huristic+=x_square;
    }
    else{
        huristic-=corner;
    }
    
    if(map[0][7]==pp) huristic+=corner;
    else if(map[7][0]==0){
        if(map[0][6]==pp || map[1][7]==pp) huristic+=c_square;
        if(map[1][6]==pp) huristic+=x_square;
    }
    else{
        huristic-=corner;
    }
    
    int cnt=0;
    for(int i=0;i<8;i++){
        if(map[i][0]==pp) cnt++;
    }
    
    if(cnt==7) huristic+=side*2*7;
    else huristic+=side*cnt;
    
    cnt=0;
    for(int i=0;i<8;i++){
        if(map[0][i]==pp) cnt++;
    }
    if(cnt==7) huristic+=side*4*7;
    else huristic+=side*cnt;
    
    cnt=0;
    for(int i=0;i<8;i++){
        if(map[7][i]==pp) cnt++;
    }
    if(cnt==7) huristic+=side*4*7;
    else huristic+=side*cnt;
    
    cnt=0;
    for(int i=0;i<8;i++){
        if(map[i][7]==pp) cnt++;
    }
    if(cnt==7) huristic+=side*4*7;
    else huristic+=side*cnt;
    
    cnt=0;
    for(int i=1;i<7;i++){
        for(int j=1;j<7;j++){
            if(map[i][j]==pp) cnt++;
        }
    }
    huristic+=cnt*15;
    val=huristic;
}
int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
int map[10][10]={-1};

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            map[i][j]=board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}
#define maxdepth 5

/*int alphabetapruning(state s,int maximum,int minimum,int depth){
 int curplayer=s.player;
 
 if(depth==maxdepth){
 s.setvalue();
 return s.val;
 }
 else{
 for(auto p:s.validspot){
 state s=state(p,curplayer,s.map);
 s.depth=depth+1;
 int value=alphabetapruning(s,s.max,s.mini,depth+1);
 if(s.depth%2==0){               //minilayer
 if(s.mini>value) s.mini=value;
 
 }
 else{                           //maxlayer
 if(s.max<value) s.max=value;
 
 }
 }
 s.depth%2==0 ? s.val=s.mini:s.val=s.max;
 return s.val;
 }
 
 
 }*/
int alphabetapruning(state s,int maximum,int minimum,int depth){
    if(depth==maxdepth){
        s.setvalue();
        return s.val;
    }
    if(s.depth%2==1){
        for(auto p:s.validspot){
            state st=state(p,s.player,s.map,depth+1);
            int value=alphabetapruning(st,maximum,minimum,st.depth);
            maximum=max(value,maximum);
            if(minimum<maximum) break;
            return value;
        }
    }
    else {
        for(auto p:s.validspot){
            state st=state(p,s.player,s.map,depth+1);
            int value=alphabetapruning(st,maximum,minimum,st.depth);
            minimum=min(minimum,value);
            if(minimum<maximum) break;
            return value;
        }
    }
    
    
    
    return 0;
}
struct comp{
    bool operator()(const state &lhs,const state &rhs) const{
        return lhs.val>rhs.val;
    }
};
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    set<state,comp> store;
    
    for(auto p:next_valid_spots){
        state newone=state(p,player,map,2);
        if(Point(0,0)==p || Point(7,0)==p || Point(7,7)==p || Point(0,7)==p ){
            newone.val=888888888;
        }
        else if((Point(1,1)==p && map[0][0]!=player) || (Point(6,6)==p && map[7][7] != player) || ( Point(1,6)==p && map[0][7] !=player ) || (Point(6,1)==p && map[7][0] != player)){
            newone.val=-88888888;
        }
        else newone.val=alphabetapruning(newone,-8888888,88888888,2);
        
        store.insert(newone);
    }
    // Remember to flush the output to ensure the last action is written to file.
    auto pt=store.begin();
    int x=pt->x;
    int y=pt->y;
    Point p=Point(x,y);
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}

