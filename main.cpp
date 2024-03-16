#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

using namespace std;

class LcgGenerator{
    uint16_t mA, mC, mCurrentX, mM;
public:
    LcgGenerator(uint16_t A, uint16_t C, uint16_t CurrentX, uint16_t M)
            :mA(A), mC(C), mCurrentX(CurrentX), mM(M){}
    uint16_t next(){
        mCurrentX=(mA*mCurrentX+mC)%mM;
        return mCurrentX;
        //va calcula si seta urmatoarea valoare pentru mCuurrentX
        //va intoarce aceasta valoare
    }
};

class Card{
    string mSuite;
    uint16_t mValue;
public:
    Card(string const &siu, uint16_t val): mSuite(siu), mValue(val){}
    bool operator==(const Card& carte)const{
        if(carte.mSuite==mSuite&&carte.mValue==mValue) return true;
        else return false;
    }
    string toString()const{
        return to_string(mValue)+','+mSuite;
    }
    bool isFake()const{
        if(mSuite!="negru"&&mSuite!="rosu"&&mSuite!="romb"&&mSuite!="trefla"||mValue<2||mValue>14) return true;
        else return false;
    }
    uint16_t getBlackjackValue() const{
        if(mValue>10) return 10;
        else return mValue;
    }
    string getSuite()const{return mSuite;}
    uint16_t getValue()const{return mValue;}
    void setSuite(string sui){mSuite=sui;}
    void setValue(uint16_t val){mValue=val;}
};

class CardPack{
    vector<Card> mCards;
    uint16_t mCurrentIndex;
public:
    CardPack(bool empty){
        if(empty== true) mCards.empty();
        else {
            for(int i=0; i<48;i++) {
                if(i<9)mCards.push_back(Card("rosu", i+2));
                if(i>8&&i<12)mCards.push_back(Card("rosu", i+3));

                if(i>11&&i<21) mCards.push_back(Card("negru", i-10));
                if(i>20&&i<24) mCards.push_back(Card("negru", i-9));

                if(i>23&&i<33) mCards.push_back(Card("romb", i-22));
                if(i>32&&i<36) mCards.push_back(Card("romb", i-21));

                if(i>35&&i<45) mCards.push_back(Card("trefla", i-34));
                if(i>44&&i<48) mCards.push_back(Card("trefla", i-33));
            }
        }
        mCurrentIndex=0;
    }
    bool goodForBackjack() const{
        for(const auto & mCard : mCards){
            if(mCard.isFake()) return false;
        }
        return true;
    }
    bool ok()const{
        if(mCards.size()!=52) return false;
        for(int i=0;i<51;i++){
            for(int j=i+1;j<52;j++){
                if(mCards[i] == mCards[j]||mCards[i].isFake()) return false;
            }
        }
        return true;
    }
    void addCard(const Card& card){
        mCards.push_back(card);
    }
    void shuffle(uint16_t a1, uint16_t c1, uint16_t x01, uint16_t a2, uint16_t c2, uint16_t x02){
        LcgGenerator lcg1(a1, c1, x01, mCards.size());
        LcgGenerator lcg2(a2, c2, x02, mCards.size());
        mCurrentIndex=0;
        uint16_t x1=0,x2=0;
        for(int i=0;i<50;i++){
            x1=lcg1.next();
            x2=lcg2.next();
            swap(mCards[x1], mCards[x2]);
        }
    }
    void printPack()const{
        for(int i=0;i<mCards.size();i++){
            cout<<mCards[i].toString();
            cout<<endl;
        }
    }
    Card nextCard(){
        return mCards[mCurrentIndex++];
    }
};

class Player{
public:
    string mName;
    uint16_t mMoney;
    uint16_t mPoints;
    Player(string name="Dealer", uint16_t money=0):mName(name), mMoney(money), mPoints(0){}
    string toString()const{
        return mName+": "+ to_string(mMoney);
    }
    void pick(const Card& card){
        mPoints+=card.getBlackjackValue();
    }
    void play(CardPack& pack){
        while(mPoints<17) pick(pack.nextCard());
    }
    void newRound(){
        mPoints=0;
    }
    bool lostRound()const{
        if(mPoints>21) return true;
        else return false;
    }
    uint16_t getPoints()const{return mPoints;}
    void operator--(){mMoney-=10;}
    void operator++(){mMoney+=10;}
    bool lostGame()const{
        if(mMoney<10) return true;
        else return false;
    }
};

int main() {
    string command;
    uint16_t card_val;
    string card_sui;
    CardPack card_pack(true);

    cin >> command;
    if(command == "check_cards"){
        while (cin>>card_val){
            getchar();
            cin>>card_sui;
            card_pack.addCard(Card(card_sui, card_val));
        }
        if (card_pack.ok()) cout<<"Pachet OK"<<endl;
        else if(card_pack.goodForBackjack()) cout<<"Pregatit pentru Blackjack"<<endl;
        else cout<<"Pachet masluit"<<endl;
    }

    uint16_t a1, c1, x01, a2, c2, x02;
    if(command == "shuffle_cards"){
        cin>>a1>>c1>>x01>>a2>>c2>>x02;
        while (cin>>card_val){
            getchar();
            cin>>card_sui;
            card_pack.addCard(Card(card_sui, card_val));
        }
//        card_pack.printPack();
        card_pack.shuffle(a1, c1, x01, a2, c2, x02);
        card_pack.printPack();
    }

    uint16_t n;
    string player_name;
    uint16_t player_money;
    vector<Player> players;
    vector<LcgGenerator> Generators;
    if(command == "play_game"){
        cin>>n;
        for(uint16_t i=0;i<n;i++){
            cin>>player_name>>player_money;
            players.push_back(Player(player_name, player_money));
        }
        CardPack pack_play(false);
        Player dealer("Dealer", 250);

        while(cin>>a1>>c1>>x01>>a2>>c2>>x02){
            pack_play.shuffle(a1, c1, x01, a2, c2, x02);
            for(int i=0;i<players.size();i++){
                players[i].pick(pack_play.nextCard());
                players[i].pick(pack_play.nextCard());
            }
            dealer.pick(pack_play.nextCard());
            dealer.pick(pack_play.nextCard());

            for (int i = 0; i < players.size(); i++) {
                players[i].play(pack_play);
            }
            dealer.play(pack_play);

            for(int i=0;i<players.size();i++) {
                if(!players[i].lostRound()&&(dealer.getPoints()<players[i].getPoints()||dealer.lostRound()))players[i].operator++();
                if (players[i].lostRound()||(!dealer.lostRound()&&players[i].getPoints() < dealer.getPoints())) players[i].operator--();

                if (!players[i].lostGame())
                    players[i].newRound();
                else {
                    auto it = players.begin() + i;
                    players.erase(it);
                    i--;
                }
            }
            dealer.newRound();
        }
        for (auto & player : players) cout<<player.toString()<<endl;
    }
    return 0;
}
