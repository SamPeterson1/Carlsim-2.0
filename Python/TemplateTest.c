int bar(void) { return 1; }

/*template<who: OINK | MEOW | MOO>*/
void aughhh() {
    /*if<who: OINK>*/
    printf("oink");
    /*elif<who: MEOW>*/
    printf("meow");
    /*elif<who: MOO>*/
    printf("moo");
    /*endif*/
}
/*endtemplate*/

/*template<foo: THIS | THAT, bar: OINK | MEOW | MOO>*/
int thisOrThat(int b) {

    /*use<aughhh>*/
    /*set<who: OINK>*/
    aughhh();

    /*if<foo: THIS>*/
        int x = 0;
        /*if<bar: OINK | MEOW>*/
        x = 1;
        /*elif<bar: MOO>*/
        x = 2;
        /*endif*/
        x += 3;
        return x;
    /*elif<foo: THAT>*/
        int x = 2;
        /*if<bar: OINK>*/
        x = 3;
        /*else*/
        x = 4;
        /*endif*/
        return x;
    /*endif*/
}
/*endtemplate*/


int main(void) {
    /*use<thisOrThat>*/
    /*set<foo: THIS, bar: MOO>*/
    int x = thisOrThat(4);
}