package it.sephiroth.androidjnisupport;

import android.app.Activity;
import android.content.Intent;
import android.widget.TextView;

import org.jetbrains.annotations.NotNull;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import androidx.test.espresso.Espresso;
import androidx.test.rule.ActivityTestRule;
import androidx.test.runner.AndroidJUnit4;
import it.sephiroth.androidjnisupport.condition_matcher.ConditionWatcher;
import it.sephiroth.androidjnisupport.condition_matcher.Instruction;

import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.withId;
import static androidx.test.espresso.matcher.ViewMatchers.withText;

@RunWith (AndroidJUnit4.class)
public class GTestInstrumented {
    public ActivityTestRule<MainActivity> activityRule = new ActivityTestRule(MainActivity.class, true, true);

    @Before
    public void setUp() {
        activityRule.launchActivity(new Intent());
    }

    @Test
    public void runAllTests() throws Exception {
        Espresso.onView(withId(R.id.button)).perform(click());
        ConditionWatcher.Companion.waitForCondition(new TextViewInstruction());
        Espresso.onView(withId(R.id.textView)).check(matches(withText("Result Code: 0")));
    }

    class TextViewInstruction extends Instruction {

        @NotNull
        @Override
        public String getDescription() {
            return "Result Code shouldn't be non empty";
        }

        @Override
        public boolean checkCondition() {
            Activity activity = activityRule.getActivity();
            TextView textView = activity.findViewById(R.id.textView);
            return textView.getText().length() > 0;
        }
    }
}
